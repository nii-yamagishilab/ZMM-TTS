import sys

from .exponential_lr import ExponentialDecayLRScheduler


def build_lr_scheduler(config):
    config_dict = config.to_dict()
    name = config_dict.pop('_name')
    return getattr(sys.modules[__name__], name)(**config_dict)