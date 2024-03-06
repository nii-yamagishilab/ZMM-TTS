import numpy as np


class ExponentialDecayLRScheduler(object):

    def __init__(self,
                 warmup_steps=50000,
                 decay_scale=50000,
                 decay_learning_rate=0.5,
                 final_learning_rate=1e-5):
        self.warmup_steps = warmup_steps
        self.decay_scale = decay_scale
        self.decay_learning_rate = decay_learning_rate
        self.final_learning_rate = final_learning_rate
    
    def get_scale(self, steps):
        if steps >= self.warmup_steps:
           scale = np.power(self.decay_learning_rate,
                            (steps - self.warmup_steps) / self.decay_scale)
        else:
           scale = 1.0
        return scale
    
    def step(self, optimizer, steps):
        scale = self.get_scale(steps)
        for key in optimizer.optimizers:
            x = optimizer.optimizers[key]
            lr = scale * optimizer.config[key].learning_rate
            lr = max(self.final_learning_rate, lr)
            for param_group in x.param_groups:
                param_group['lr'] = lr