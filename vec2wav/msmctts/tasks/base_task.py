import torch

from msmctts.networks import find_modules


class BaseTask(torch.nn.Module):

    def __init__(self, config, mode='train'):
        super().__init__()
        self.config = config
        self.mode = mode
        modules = config.task.network if hasattr(config.task, 'network') \
            else {k: v for k ,v in config.task.items()
                  if k[: 1] != '_' and '_name' in v}        
        for name, network in find_modules(modules):
            self.add_module(name, network)

    def forward(self, features):
        func = {
            'train': self.train_step,
            'infer': self.infer_step,
            'debug': self.debug_step,
        }[self.mode]
        return func(features)
    
    def train_step(self, features):
        pass

    def infer_step(self, features):
        pass

    def debug_step(self, features):
        pass
