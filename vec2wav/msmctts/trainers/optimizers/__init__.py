from torch.optim import Adam, AdamW

import re

from .radam import RAdam


def get_optimizer(parameters, config):
    name = config._name
    if name == 'RAdam':
        optimizer = RAdam(parameters, config.learning_rate, config.betas,
                          config.eps, config.weight_decay)
    elif name == 'Adam':
        optimizer = Adam(parameters, config.learning_rate, config.betas,
                         config.eps, config.weight_decay)
    elif name == 'AdamW':
        optimizer = AdamW(parameters, config.learning_rate, config.betas,
                          config.eps, config.weight_decay)

    
    return optimizer


def build_optimizer(model, config):
    optimizer_dict, config_dict = {}, {}
    
    for module_name, module in model.named_children():
        # Check if module name or "_default" is in config
        try:
            module_config = config[module_name]
        except KeyError:
            assert hasattr(config, '_default'), \
                   'Both {} and _default not found'.format(module_name)
            module_config = config._default
        config_dict[module_name] = module_config

        # Load specific parameters if 'parameters' in config[key]
        parameters = module.parameters()
        if hasattr(module_config, 'parameters'):
            parameters = []
            for name, parameter in module.named_parameters():
                if re.match(module_config.parameters, name):
                    parameters.append(parameter)
                else:
                    parameter.requires_grad = False
        
        # Build optimizer for module
        optimizer_dict[module_name] = get_optimizer(parameters, module_config)
    
    return Optimizer(optimizer_dict, config_dict)


class Optimizer(object):
    def __init__(self, optimizers_dict, config):
        self.optimizers = optimizers_dict
        self.config = config

    def load_state_dict(self, parameters_dict):
        for key in self.optimizers:
            self.optimizers[key].load_state_dict(parameters_dict[key])

    def state_dict(self):
        parameters_dict = {}
        for key in self.optimizers:
            parameters_dict[key] = self.optimizers[key].state_dict()
        return parameters_dict

    def zero_grad(self, names=None):
        names = tuple(self.optimizers.keys()) if names is None else names
        names = [names] if type(names) not in (list, tuple) else names
        for key in names:
            self.optimizers[key].zero_grad()

    def step(self, names=None):
        names = tuple(self.optimizers.keys()) if names is None else names
        names = [names] if type(names) not in (list, tuple) else names
        for key in names:
            self.optimizers[key].step()

