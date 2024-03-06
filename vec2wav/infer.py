from tqdm import tqdm
from scipy.io.wavfile import write
from torch.multiprocessing import set_start_method
from torch.utils.data import DataLoader

import argparse
import numpy as np
import os
import re
import torch

from msmctts.datasets import build_dataset
from msmctts.tasks import build_task
from msmctts.utils.plot import plot_matrix
from msmctts.utils.utils import to_model, feature_normalize

try:
    set_start_method('spawn')
except RuntimeError:
    pass


def get_output_base_path(checkpoint_path):
    base_dir = os.path.dirname(checkpoint_path)
    match = re.compile(r'.*_([0-9]+)').match(checkpoint_path)
    name = 'eval-%d' % int(match.group(1)) if match else 'eval'
    return os.path.join(base_dir, name)


def save_feature(path, feat, format, sample_rate):
    if format == '.npy':
        print ('--path---',path)
        print ('---feat',feat)
        np.save(path, feat)
    elif format == '.png':
        plot_matrix(feat, path)
    elif format == '.txt':
        np.savetxt(path, feat, fmt="%.6f")
    elif format == '.dat':
        feat.astype(np.float32).tofile(path)
    elif format == '.wav':
        peak = max(abs(feat))
        feat = feat / peak if peak > 1 else feat
        write(path, sample_rate, (feat * 32767.0).astype(np.int16))


def test(task, testset, output_dir, n_jobs=1):
    dataloader = DataLoader(testset, batch_size=n_jobs,
        num_workers=0, shuffle=False, pin_memory=False, drop_last=False,
        sampler=torch.utils.data.SequentialSampler(testset),
        collate_fn=(testset.collate_fn
            if hasattr(testset, 'collate_fn') else None)
    )

    # Startup task
    if torch.cuda.is_available():
        task = task.cuda()
    task.eval()
    
    # Build output directories
    if not hasattr(task.config, 'save_features'):
        raise ValueError("No saved features")
    
    feat_dir = {}
    for name, _, _ in task.config.save_features:
        feat_dir[name] = os.path.join(output_dir, name)
        os.makedirs(feat_dir[name], exist_ok=True)

    # Multi-process or Single-process
    for batch_i, features in tqdm(enumerate(dataloader)):
        # Get IDs if the batch is sorted in collate_fn
        test_ids = [testset.id_list[x] for x in features.pop('_id')]

        # Model inference
        features = to_model(features)
        #print (features)
        saved_features = task(features)
        print ('--saved_features----',saved_features)
        # Save output features
        for i, test_id in enumerate(test_ids):
            for name, fmt, sample_rate in task.config.save_features:
                # Convert feature to numpy
                feat = saved_features[name][i]
                print ('--feat----',feat)
                if isinstance(feat, torch.Tensor):
                    feat = feat.detach().cpu().numpy()

                # Denormalize (optional)
                if name in testset.feature_stat:
                    stat = testset.feature_stat[name]
                    feat = feature_normalize(feat, stat, True)
                
                # Save feature
                 
                path = "{}/{}{}".format(feat_dir[name][:-3], test_id,fmt)
                print (feat_dir[name])
                print (test_id)
                print (fmt)
                print ('---path----',path)
                save_feature(path, feat, fmt, sample_rate=sample_rate)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', "--model", required=True)
    parser.add_argument('-c', "--config", default=None)
    parser.add_argument('-t', "--test_config", default=None)
    parser.add_argument('-j', "--jobs", type=int, default=1)
    parser.add_argument('-o', "--output_dir", default=None)
    parser.add_argument("--debug", action='store_true')
    args = parser.parse_args()
    
    # Check arguments
    if args.model is None and args.config is None:
        parser.error('at least one argument shoule be given: -m/--model, -c/--config')

    # Load task from checkpoint file
    task = build_task(args.config,
                      mode='debug' if args.debug else 'infer',
                      checkpoint=args.model)

    # Build Test Dataset
    dataset_config = task.config.testset \
        if hasattr(task.config, 'testset') else \
        task.config.dataset
    dataset_config['training'] = False
    if args.test_config is not None:
        dataset_config['id_list'] = args.test_config
    dataset = build_dataset(dataset_config)

    # Auto-generate output directory
    if args.output_dir is None:
        args.output_dir = get_output_base_path(args.model) \
            if args.model is not None else os.path.dirname(args.config)
    os.makedirs(args.output_dir, exist_ok=True)

    # Inference
    test(task, dataset, args.output_dir, n_jobs=args.jobs)


if __name__ == '__main__':
    main()
