from concurrent.futures import ThreadPoolExecutor
from functools import partial
from multiprocessing import Pool
from multiprocessing import cpu_count
from typing import Type
from tqdm import tqdm

import array
import io
import math
import numpy as np
import os
import pickle
import random
import torch
import zipfile
import json


from msmctts.utils.config import load_yaml
from msmctts.utils.utils import load_numpy_file, load_wav_file, feature_normalize


MIN_DATASET_SIZE = 3200


class BaseDataset(torch.utils.data.Dataset):
    
    def __init__(self, id_list, feature, samplerate, dimension, frameshift,
                 feature_path=None, feature_stat=None, padding_value=None,
                 segment_length=-1, pre_load=True, seed=1234, training=True):
        super().__init__()
        self.samplerate = samplerate
        self.feature = feature
        self.dimension = {f: d for f, d in zip(feature, dimension) if d > 0}
        self.frameshift = {f: s for f, s in zip(feature, frameshift)
                           if s is not None and s > 0}
        self.padding_value = {f: d for f, d in zip(feature, padding_value)} \
            if padding_value is not None else {f: 0 for f in feature}
        self.segment_length = segment_length
        self.pre_load = pre_load
        self.training = training
        self.dataset = dict()
        
        '''
        with open('Dataset/Group1v2/vq_speaker_w_val_wsin.json', 'r') as f:
            self.speaker_dict = json.load(f)
        '''
        with open('Dataset/preprocessed_data/ZMM6/language_dict.json', 'r') as f:
            self.language_dict = json.load(f)
        
        self.language_dict_static={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
        # Get feature statistics (optional)
        self.feature_stat = {}
        if feature_stat is not None:
            self.feature_stat = {f: load_yaml(d)
                for f, d in zip(feature, feature_stat) if d is not None}

        # Initial random seed
        random.seed(seed)

        # Parse file list
        id_list = self.prepare_dataset(id_list, feature_path)
        self.id_list = id_list

    def __len__(self):
        if self.training:
            return max(MIN_DATASET_SIZE, len(self.id_list))
        return len(self.id_list)
    
    def __getitem__(self, index):
        return self.parse_case(index % len(self.id_list))

    def parse_case(self, index):
        # Load items from dataset dictionary
        data_dict = {feat: self.dataset[(self.id_list[index], feat)]
                     for feat in self.feature
                     if (self.id_list[index], feat) in self.dataset}
        
        
        #print (data_dict)
        #basename = data_dict['mel'].split('/')[-1].split('.')[0]
        #print (basename)
        basename = data_dict['mel'].split('/')[-1].split('.')[0]
        if data_dict['mel'].split('/')[0]=='..':
            #Just for extracting code
            language_id=0
            #speaker_emb = np.load(speaker_path)
            speaker_path= ('/home/others/v-cheng-gong/Low_resources_languages/Model_For_Group1_v2/Dataset/preprocessed_data/Group1v2/SpeakerEmb/LJ001-0001.npy')
            speaker_emb = np.load(speaker_path)
        else: 
         if basename.split('_')[0] == 'Generate':
           speaker_name =  '_'.join(basename.split('_')[3:])
           speaker_path = os.path.join('Dataset/MM6/test_spk_emb/',
                                 "{}.npy".format(speaker_name),)
           speaker_emb = np.load(speaker_path)
           language_id = int(self.language_dict_static[basename.split('_')[1]])
         else:
              speaker_path = os.path.join('Dataset/preprocessed_data/ZMM6',
              "SpeakerEmb",
              "{}.npy".format(basename),)

              speaker_emb = np.load(speaker_path)
              language_id = int(self.language_dict[basename]) 
        
        # Find sequence duration for all time-sequences
        dur, dur_s = -1, 0
        if self.training and self.segment_length > 0:
            dur = self.segment_length
            feat = max(self.frameshift, key=self.frameshift.get)
            shape = data_dict[feat].shape if self.pre_load else \
                    self.parse_file(data_dict[feat], self.dimension[feat],
                                    return_shape=True)
            ind_e = max(0, shape[0] - math.ceil(dur / self.frameshift[feat]))
            dur_s = 1. * random.randint(0, ind_e) * self.frameshift[feat]
        
        # Load all items
        for key in data_dict.keys():
            feature = data_dict[key]

            # Get start index and length
            start, length = 0, -1
            if key in self.frameshift:
                start = int(dur_s / self.frameshift[key])
                length = int(dur / self.frameshift[key])

            # Parse feature
            if isinstance(feature, (list, tuple, np.ndarray)):
                end = start + length if length > 0 else None
                feature = feature[start: end]
            elif isinstance(feature, str):
                string = feature
                func = self.parse_file if os.path.isfile(string) else \
                       self.parse_string
                feature = func(string, dimension=self.dimension[key],
                               start=start, length=length)
                if 0 in feature.shape:
                    raise ValueError("Cannot parse string: {}".format(string))
            else:
                raise TypeError("Unknown feature type: {}".format(type(feature)))
            
            # Normalize features (optional)
            if key in self.feature_stat:
                stat = self.feature_stat[key]
                feature = feature_normalize(feature, stat)

            data_dict[key] = feature
        
        if not self.training:
            data_dict['_id'] = index
        #print ('------------')
        #print (data_dict)
        data_dict['language'] = int(language_id)
        data_dict['speaker']  = speaker_emb
        
        #print (data_dict)
        return data_dict

    def parse_file(self, path, dimension=None, start=0, length=-1, return_shape=False):
        if not hasattr(self, 'ext_dict'):
            ext_dict = {
                'parse_numpy_file': ['.npy'],
                'parse_dat_file': ['.dat', '.mgc', '.ap'],
                'parse_audio_file': ['.wav'],
                'parse_torch_file': ['.pt'],
            }
            self.ext_dict = {}
            for k, lv in ext_dict.items():
                self.ext_dict.update({v: getattr(self, k) for v in lv})
        ext_func = self.ext_dict[os.path.splitext(path)[-1]]

        # Parse compressed file
        if not os.path.isfile(path) and ':' in path:
            file_zip, file_data = path.split(':')
            zip_ext = os.path.splitext(file_zip)[-1]
            if not hasattr(self, 'file_handles'):
                self.file_handles = {}
            if file_zip not in self.file_handles:
                self.file_handles[file_zip] = zipfile.ZipFile(file_zip, 'r')
            with self.file_handles[file_zip].open(file_data, 'r') as zip_data:
                with io.BytesIO(zip_data.read()) as buffer:
                    data = ext_func(buffer,
                                    dimension=dimension,
                                    start=start,
                                    length=length,
                                    return_shape=return_shape)
        else:
            data = ext_func(path,
                            dimension=dimension,
                            start=start,
                            length=length,
                            return_shape=return_shape)
        return data

    def parse_string(self, string,
                     dimension=None, start=0, length=-1, return_shape=False):
        # Parse string to float array
        if '_' in string:
            string = string.replace('_', ' ')
        x = np.fromstring(string, sep=' ')
        if dimension is not None:
            x = np.reshape(x, (len(x) // dimension, dimension))
        if return_shape:
            return x.shape
        x = x[start: length if length > 0 else None]
        return x

    def parse_book(self, path, id_list=None, feat=None):
        print("Loading book: {}".format(path))
        
        if os.path.splitext(path)[-1] in ('.list', '.txt'):
            with open(path) as fin:
                data = [x.strip().split('|') for x in fin.readlines()]
            
            book = {}
            for segs in data:
                case_id, feats_list = segs[0], []
                for feats in segs[1:]:
                    array = np.array([
                        float(feat) if '_' not in feat else
                            [float(x) for x in feat.split('_')]
                        for feat in feats.split(' ')])
                    feats_list.append(array)
                feats = feats_list if len(feats_list) > 1 else feats_list[0]
                book[case_id] = feats
        elif os.path.splitext(path)[-1]  == '.pkl':
            with open(path, 'rb') as fin:
                book = pickle.load(fin)
        elif os.path.splitext(path)[-1]  == '.yaml':
            book = load_yaml(path)
        
        if id_list is not None:
            for attrs in id_list:
                attr = [attr for attr in attrs if attr in book][0]
                self.dataset.update({(attrs, feat): np.asarray(book[attr])})
        
        return book

    def parse_audio_file(self, path,
                         dimension=None, start=0, length=-1, return_shape=False):
        x = load_wav_file(path, start=start, length=length, return_shape=return_shape)
        return x if return_shape else np.expand_dims(x[0], axis=-1)

    def parse_numpy_file(self, path,
                         dimension=None, start=0, length=-1, return_shape=False):
        return load_numpy_file(path, start=start, length=length, return_shape=return_shape)

    def parse_torch_file(self, path,
                         dimension=None, start=0, length=-1, return_shape=False):
        data = torch.load(path).squeeze(0).numpy()

        if dimension is not None and data.shape[0] == dimension:
            data = np.transpose(data)

        if return_shape:
            return data.shape

        data = data[start:]
        if length > 0:
            data = data[: length]

        return data
       
    def parse_dat_file(self, path,
                       dimension=None, start=0, length=-1, return_shape=False):
        data = array.array('f')
        with open(path, 'rb') as fin:
           n = os.fstat(fin.fileno()).st_size // 4
           data.fromfile(fin, n)
        if dimension is None:
           dimension = 1
        data = np.frombuffer(data, dtype=np.float32).reshape(-1, dimension)
        return data

    def prepare_dataset(self, id_list_file, feature_path):
        # "id_list_file" can be either string or list
        # E.g. 'dataset_path/id.list' or ['dataset_path1/id.list', 'dataset_path2/id.list']
        if isinstance(id_list_file, (tuple, list)):
            id_list = []
            for i in range(len(id_list_file)):
                list_file = id_list_file[i]
                path = [x[i] for x in feature_path]
                id_list = id_list + self.prepare_dataset(list_file, path)
            return id_list

        # Parse ID list file
        if '.yaml' in id_list_file:
            data_dict = load_yaml(id_list_file)
            print (data_dict)
            id_list = sorted(data_dict.keys())
            for case_id in id_list:
                feats = data_dict[case_id]
                for name, item in feats.items():
                    self.dataset[(case_id, name)] = item
        else:
            with open(id_list_file) as fin:
                id_list = [tuple(x.strip().split()) for x in fin.readlines()]
            for feat, path in zip(self.feature, feature_path):
                if isinstance(path, str) and os.path.isfile(path):
                    self.parse_book(path, id_list=id_list, feat=feat)
                    continue
                feat_paths = [path.format(*attrs) for attrs in id_list]
                self.dataset.update({(attrs, feat): feat_path
                    for attrs, feat_path in zip(id_list, feat_paths)})
        
        if self.pre_load and self.training:
            self.preload_files()
        
        if self.training:
            random.shuffle(id_list)
       
        #print (id_list)
        return id_list

    def preload_files(self):
        for feat in self.feature:
            keys = [key for key in self.dataset.keys() if feat == key[-1]]
            args = [(self.dataset[key], self.dimension[feat]) for key in keys]
            if type(args[0][0]) != str:
                continue
            datas = self.parallel(self.parse_file, args)
            self.dataset.update({key: data for key, data in zip(keys, datas)})
            
    def parallel(self, func, args, max_workers=cpu_count() // 2, use_tqdm=True):
        with ThreadPoolExecutor(max_workers=max_workers) as executor:
            futures = [executor.submit(partial(func, *arg)) for arg in args]
            futures = tqdm(futures) if use_tqdm else futures
            results = [future.result() for future in futures]
        return results
        
