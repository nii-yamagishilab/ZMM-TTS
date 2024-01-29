from os.path import dirname
from torch.utils.data import DataLoader
from torch.utils.data.distributed import DistributedSampler

from msmctts.utils.utils import module_search


def build_dataset(config):
    dataset_name = config['_name']
    config_dataset = {k: v for k, v in config.items() if k[: 1] != '_'}
    DatasetClass = module_search(dataset_name, dirname(__file__), 'msmctts.datasets')
    dataset = DatasetClass(**config_dataset)
    return dataset


def build_dataloader(config_dataset, config_dataloader, distributed=False):
    dataset = build_dataset(config_dataset)

    data_sampler = DistributedSampler(dataset) if distributed else None

    collate_fn=(dataset.collate_fn if hasattr(dataset, 'collate_fn') else None)
    data_loader = DataLoader(
            dataset,
            num_workers=config_dataloader.num_workers,
            collate_fn=collate_fn,
            shuffle=(data_sampler is None),
            sampler=data_sampler,
            batch_size=config_dataloader.batch_size,
            pin_memory=False,
            drop_last=True,
            prefetch_factor=2,
            persistent_workers=False)
    
    return dataset, data_sampler, data_loader
