import os
import re
import yaml


DEFAULT_DICT = {
    # Configuration ID
    'id': "null",
    # Training configuration
    'save_checkpoint_dir': "",
    'pretrain_checkpoint_path': "",
    'restore_checkpoint_path': "",
    'resume_training': True,
    'training_steps': 1000000,
    'iters_per_checkpoint': 50000,
    'seed': 1234,
    # CuDNN configuration
    'cudnn': {
        'enabled': True,
        'benchmark': False,
    },
    # Only for Multi-GPU Training
    'distributed': {
        "dist_backend": "nccl",
        "dist_url": "tcp://localhost:54321",
    }
}


def load_yaml(yaml_file):
    loader = yaml.SafeLoader
    loader.add_implicit_resolver(
        u'tag:yaml.org,2002:float',
        re.compile(u'''^(?:
        [-+]?(?:[0-9][0-9_]*)\\.[0-9_]*(?:[eE][-+]?[0-9]+)?
        |[-+]?(?:[0-9][0-9_]*)(?:[eE][-+]?[0-9]+)
        |\\.[0-9_]+(?:[eE][-+][0-9]+)?
        |[-+]?[0-9][0-9_]*(?::[0-5]?[0-9])+\\.[0-9_]*
        |[-+]?\\.(?:inf|Inf|INF)
        |\\.(?:nan|NaN|NAN))$''', re.X),
        list(u'-+0123456789.'))
    with open(yaml_file) as f:
        config_dict = yaml.load(f, Loader=loader)
    return config_dict
    

class ConfigItem(dict):
    __slots__ = ()

    def __init__(self, config_dict=None):
        if config_dict is None:
            config_dict = dict()
        if isinstance(config_dict, ConfigItem):
            config_dict = config_dict.to_dict()
        assert isinstance(config_dict, dict)

        # Set attributes (not dict in ConfigItem)
        for key, value in config_dict.items():
            if isinstance(value, (list, tuple)):
                value = [ConfigItem(x) if isinstance(x, dict) else x for x in value]
            elif isinstance(value, dict):
                value = ConfigItem(value)
            elif isinstance(value, ConfigItem):
                value = ConfigItem(value.to_dict())
            elif isinstance(value, str) and value.lower() == 'none':
                value = None
            self[key] = value
    
    def __getattr__(self, item):
        try:
            return self[item]
        except KeyError:
            raise AttributeError(item)
    
    def __setattr__(self, name, value):
        self[name] = value
    
    def to_dict(self, recursive=True):
        conf_dict = {}
        for k, v in self.items():
            if isinstance(v, ConfigItem) and recursive:
                v = v.to_dict(recursive)
            conf_dict[k] = v
        return conf_dict

    def update(self, obj):
        assert isinstance(obj, (ConfigItem, dict))

        for k, v in obj.items():
            if k not in self or not isinstance(v, (ConfigItem, dict)):
                self[k] = v
            else:
                self[k].update(v)


class Config(ConfigItem):
    def __init__(self, yaml_object):
        super().__init__(DEFAULT_DICT)
        
        # Check yaml_object
        if isinstance(yaml_object, str):
            assert os.path.isfile(yaml_object)
            yaml_object = load_yaml(yaml_object)

        if isinstance(yaml_object, dict):
            yaml_object = ConfigItem(yaml_object)
        
        assert isinstance(yaml_object, ConfigItem)

        self.update(yaml_object)
