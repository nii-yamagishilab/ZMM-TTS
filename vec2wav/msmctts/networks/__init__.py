import os

from msmctts.utils.utils import module_search


def find_modules(conf):
    module_names, confs = zip(*conf.items())
    names = [x['_name'] for x in confs]
    confs = [{k: v for k ,v in conf.items() if k[: 1] != '_'} for conf in confs]
    modules = module_search(names, os.path.dirname(__file__), 'msmctts.networks')
    return [(x, modules[i](**confs[i])) for i, x in enumerate(module_names)]