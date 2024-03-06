from tensorboardX import SummaryWriter

import logging
import os
import sys
import time


def init_logger(filename=None, stdout=True):
    logging.getLogger().handlers.clear()
    
    logger = logging.getLogger(None)
    logger.setLevel(logging.INFO)

    if not logger.handlers:
        formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")
        
        fh = logging.FileHandler(filename, mode='a')
        fh.setLevel(logging.INFO)
        fh.setFormatter(formatter)
        logger.addHandler(fh)

        if stdout:
            ch = logging.StreamHandler()
            ch.setLevel(logging.INFO)
            ch.setFormatter(formatter)
            logger.addHandler(ch)

    return logger


class Logger(object):
    def __init__(self, directory, event_dir='', log_file='train.log'):
        if not os.path.isdir(directory):
            os.makedirs(directory, exist_ok=True)
            os.chmod(directory, 0o775)
        print("Output directory: ", directory)
        event_dir = os.path.join(directory, "{}{}".format(
            event_dir, time.strftime("%Y-%m-%d_%H-%M-%S", time.localtime())))
        os.makedirs(event_dir, exist_ok=True)
       
        self.logger = init_logger(os.path.join(directory, log_file), stdout=False)
        self.event_writer = SummaryWriter(logdir=event_dir)
        self.meters = {}
    
    def log(self, iteration, log, stdout=False):    
        # Save images
        if 'image' in log:
            self.add_image(iteration, log['image'])
        if 'audio' in log:
            self.add_audio(iteration, log['audio'], log['sample_rate'])
        
        # Save loss information
        loss = log['loss']
        message = "{:>7}: ".format(iteration)
        for key in loss:
            value = loss[key].item() if hasattr(loss[key], 'item') else loss[key]
            message += "{}: {:>.3e}, ".format(key, value)
            if key not in self.meters:
                self.meters[key] = LossMeter(key, self.event_writer, 100, True)
            self.meters[key].add(iteration, value)
        self.logger.info(message)
        if stdout:
            print(message)

    def info(self, message):
        self.logger.info(message)

    def add_image(self, iteration, images):
        for key in images:
            self.event_writer.add_image(key, images[key], iteration, dataformats='HWC')

    def add_audio(self, iteration, audios, sample_rate):
        for key in audios:
            self.event_writer.add_audio(key, audios[key], iteration, sample_rate)


class LossMeter():

    def __init__(self, name, writer, log_per_step, auto_log=True):
        self.name = name
        self.writer = writer
        self.log_per_step = log_per_step
        self.auto_log = auto_log
        self.loss = []

    def add(self, step, loss):
        assert isinstance(loss, (float, int)), 'Loss must be float type'
        self.loss.append(loss)

        if self.auto_log and step % self.log_per_step == 0:
            self.writer.add_scalar(self.name, self.mean(), step)
            self.reset()

    def reset(self):
        self.loss = []

    def mean(self):
        return self.sum() / len(self.loss)

    def sum(self):
        return sum(self.loss)
