from os.path import dirname

from msmctts.utils.utils import module_search


def build_trainer(config, model, num_gpus=1, rank=0):
    # Build Trainer
    trainer_config_dict = config.trainer.to_dict()
    name = trainer_config_dict.pop('_name')
    Trainer = module_search(name, dirname(__file__), 'msmctts.trainers')
    trainer = Trainer(config, model, num_gpus=num_gpus, rank=rank, **trainer_config_dict)
    return trainer