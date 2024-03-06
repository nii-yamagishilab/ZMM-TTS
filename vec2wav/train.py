import argparse
import os
import torch

from msmctts.distributed.distributed import init_distributed, apply_gradient_allreduce
from msmctts.utils.config import Config
from msmctts.tasks import build_task
from msmctts.trainers import build_trainer


def train(config, num_gpus, rank, group_name):
    torch.manual_seed(config['seed'])
    torch.cuda.manual_seed(config['seed'])
    
    # =============== INIT MUTLI-GPU ENVIRONMENT ===============
    if num_gpus > 1:
        init_distributed(rank, num_gpus, group_name,
                         **config.distributed)
        config.dataloader.batch_size = config.dataloader.batch_size // num_gpus
        print(f"Batch size per GPU is changed to {config.dataloader.batch_size}.")
  
    # =============== CONSTRUCT TASK AND TRAINER ===============
    task = build_task(config, 'train')
    trainer = build_trainer(config, task, num_gpus=num_gpus, rank=rank)

    # =================== MAIN TRAINNING LOOP ==================
    trainer.train()
    
    print("Training done!")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--config', type=str, required=True,
                        help='YAML file for configuration')
    parser.add_argument('-r', '--rank', type=int, default=0,
                        help='rank of process for distributed')
    parser.add_argument('-g', '--group_name', type=str, default='',
                        help='name of group for distributed')
    args = parser.parse_args()
    
    # Parse configs.
    config = Config(args.config)

    # Construct directory for saving checkpoints
    if 'save_checkpoint_dir' not in config:
        ckpt_dir = os.path.join(os.path.dirname(args.config), "checkpoints")
        config.save_checkpoint_dir = ckpt_dir # os.path.join(ckpt_dir, config.id)

    # Single GPU or Multi GPU
    num_gpus = torch.cuda.device_count()
    if num_gpus > 1:
        if args.group_name == '':
            print("WARNING: Multiple GPUs detected but no distributed group set")
            num_gpus = 1
    if num_gpus == 1 and args.rank != 0:
        raise Exception("Doing single GPU training on rank > 0")
    
    # Set CuDNN configuration
    torch.backends.cudnn.enabled = True
    torch.backends.cudnn.benchmark = True
    if 'cudnn' in config:
        torch.backends.cudnn.enabled = config.cudnn.enabled
        torch.backends.cudnn.enabled = config.cudnn.benchmark

    # Begin Training
    train(config, num_gpus, args.rank, args.group_name)


if __name__ == '__main__':
    main()