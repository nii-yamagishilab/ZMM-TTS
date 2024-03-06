from tqdm import tqdm

import glob
import json
import re
import torch

from msmctts.datasets import build_dataloader
from msmctts.distributed.distributed import *
from msmctts.utils.logger import Logger
from msmctts.utils.utils import to_model, load_checkpoint
from .lr_schedulers import build_lr_scheduler
from .optimizers import build_optimizer


class BaseTrainer(object):

    def __init__(self, config, model, num_gpus=1, rank=0):
        self.config = config
        self.distributed = (num_gpus > 1)
        self.rank = rank
        self.fp16_training = False

        # Adjust model
        if hasattr(self.config, 'freeze') and self.config.freeze != '':
            for name, param in model.named_parameters():
                if re.match(self.config.freeze, name):
                    param.requires_grad = False
        if num_gpus > 0:
            model = model.cuda()
        if self.distributed:
            model = apply_gradient_allreduce(model)
        self.model = model

        if self.fp16_training:
            self.scaler = torch.cuda.amp.GradScaler()

    def train(self):
        # Build Dataset
        _, data_sampler, data_loader = build_dataloader(
                self.config.dataset, self.config.dataloader, self.distributed)
        
        # Build Optimizer
        self.optimizer = build_optimizer(self.model, self.config.optimizer)
        
        # Construct learning rate decay strategy
        lr_scheduler = build_lr_scheduler(self.config.lr_scheduler)

        # Load checkpoint if one exists
        iteration = self.attempt_load_checkpoint()

        # Construct logger
        if self.distributed:
            logger = Logger(self.config.save_checkpoint_dir,
                            'GPU_{}_'.format(self.rank),
                            'GPU_{}.log'.format(self.rank))
        else:
            logger = Logger(self.config.save_checkpoint_dir)
        logger.info(json.dumps(self.config.to_dict(), indent=2))

        # Construct progress bar
        pbar_description = "Epoch {}, Iter {} ({}/{})"
        pbar = tqdm(total=self.config.training_steps - iteration,
                    bar_format='{desc} [{elapsed}, {rate_fmt}{postfix}]')

        # ================ TRAIN ===================
        self.model.train()
        while True:
            
            epoch = iteration // len(data_loader)
            if data_sampler is not None:
                data_sampler.set_epoch(epoch)

            for i, batch in enumerate(data_loader):
                if self.rank == 0:
                    pbar.set_description_str(pbar_description.format(
                        epoch, iteration, i, len(data_loader)))
                    pbar.update(1)
                
                lr_scheduler.step(self.optimizer, iteration)
                
                batch = to_model(batch)
                
                self.model.zero_grad()
                self.optimizer.zero_grad()
                log = self.train_step(batch, iteration)
                
                logger.log(iteration, log)
                if self.rank == 0 and iteration > 0 and \
                        iteration % self.config.iters_per_checkpoint == 0:
                    checkpoint_path = "{}/model_{}".format(
                        self.config.save_checkpoint_dir, iteration)
                    self.save_checkpoint(checkpoint_path, iteration)
                
                if iteration >= self.config.training_steps:
                    pbar.close()
                    return
                
                iteration += 1
    
    def train_step(self, batch, iteration):
        pass

    def attempt_load_checkpoint(self):
        restore_checkpoint_path = self.config.restore_checkpoint_path
        latest_checkpoint_path = self.find_latest_checkpoint()
        if self.config.resume_training and latest_checkpoint_path != '':
            restore_checkpoint_path = latest_checkpoint_path

        if restore_checkpoint_path != "":
            iteration = load_checkpoint(restore_checkpoint_path,
                                        self.model, self.optimizer)
            iteration += 1  # next iteration is iteration + 1
        else:
            iteration = 0
            if self.config.pretrain_checkpoint_path != '':
                load_checkpoint(self.config.pretrain_checkpoint_path, self.model)

        return iteration

    def find_latest_checkpoint(self):
        directory = self.config.save_checkpoint_dir
        if not os.path.exists(directory):
            return ""
        
        checkpoints = glob.glob(os.path.join(directory, 'model_*'))
        if len(checkpoints) == 0:
            return ""
        
        max_iterations = max([int(x.split('_')[-1]) for x in checkpoints])
        if max_iterations == 0:
            return ""
        
        return os.path.join(directory, 'model_' + str(max_iterations))

    def save_checkpoint(self, filepath, iteration):
        print("Saving at iteration {} to {}".format(iteration, filepath))
        torch.save({'model': self.model.state_dict(),
                    'optimizer': self.optimizer.state_dict(),
                    'iteration': iteration,
                    'config': self.config.to_dict()}, filepath,
                    _use_new_zipfile_serialization=False)
