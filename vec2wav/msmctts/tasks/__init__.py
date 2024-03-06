from os.path import dirname

import torch

from msmctts.utils.config import Config
from msmctts.utils.utils import module_search, load_checkpoint


def load_model(name, checkpoint_path, config_path=None):
    task = load_task(checkpoint_path, config_path)
    return getattr(task, name)


def load_task(checkpoint_path, config_path=None, mode='infer'):
    checkpoint = torch.load(checkpoint_path, map_location='cpu')
    model_config = Config(config_path
                          if config_path is not None
                          else checkpoint['config'])
    # Build task from config
    task = build_task(model_config, mode)
    iter = load_checkpoint(checkpoint, task)
    return task


def build_task(config=None, mode='train', checkpoint=None, *args, **kwargs):
    # Check valid arguments
    assert config is not None or checkpoint is not None

    # Load task from checkpoint file
    if checkpoint is not None:
        return load_task(checkpoint, config, mode)
    
    # Check type of config
    if isinstance(config, str):
        config = Config(config)
    assert type(config) == Config
    
    print (config)

    # Init Task
    task_name = config.task._name
    TaskClass = module_search(task_name, dirname(__file__), 'msmctts.tasks')
    task = TaskClass(config, mode=mode, *args, **kwargs)

    return task
