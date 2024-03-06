from numpy.lib import format
from soundfile import SoundFile

import contextlib
import glob
import importlib
import io
import inspect
import numpy as np
import os
import re
import torch
import soundfile as sf
from torch.nn import modules


'''
Feature IO & processing
'''
def load_numpy_file(path, start=0, length=-1, return_shape=False):
    pickle_kwargs = dict(encoding='ASCII', fix_imports=True)

    with contextlib.ExitStack() as stack:
        if hasattr(path, 'read'):
            fid = path
            own_fid = False
        else:
            fid = stack.enter_context(open(path, "rb"))
            own_fid = True

        # Code to distinguish from NumPy binary files and pickles.
        _ZIP_PREFIX = b'PK\x03\x04'
        _ZIP_SUFFIX = b'PK\x05\x06' # empty zip files start with this
        N = len(format.MAGIC_PREFIX)
        magic = fid.read(N)
        # If the file size is less than N, we need to make sure not
        # to seek past the beginning of the file
        fid.seek(-min(N, len(magic)), 1)  # back-up
        
        version = format.read_magic(fid)
        format._check_version(version)
        shape, fortran_order, dtype = format._read_array_header(fid, version)
        count = 1 if len(shape) == 0 else np.multiply.reduce(shape, dtype=np.int64)

        if return_shape:
            return shape

        assert start < shape[0]
        if dtype.itemsize <= 0:
            raise ValueError('Wrong dtype: {}'.format(dtype))

        if fortran_order and start >= 0 and length > 0:
            if len(shape) != 2:
                raise RuntimeError("Only support 2-dim Fortran matrix!")

            length = min(length, shape[0] - start)
            slice_count = length * shape[1]
            slice_shape = (length, shape[1])

            array = np.ndarray(slice_count, dtype=dtype)
            for i in range(shape[1]):
                read_size = int(length * dtype.itemsize)
                fid.seek(start * dtype.itemsize, 1)
                data = format._read_bytes(fid, read_size, "array data")
                fid.seek((shape[0] - (start + length)) * dtype.itemsize, 1)
                array[i * length: (i + 1) * length] = np.frombuffer(data,
                                                                    dtype=dtype,
                                                                    count=length)
            array.shape = slice_shape[::-1]
            array = array.transpose()
            return array

        # Read with offset, 1st order must be time
        if start >= 0 and length > 0 and len(shape) > 0:
            length = min(length, shape[0] - start)
            single_shape = list(shape[1: ]) if len(shape) > 1 else [1]
            offset = np.multiply.reduce([start] + single_shape, dtype=np.int64)
            count = np.multiply.reduce([length] + single_shape, dtype=np.int64)
            fid.seek(offset * dtype.itemsize, 1)
            shape = tuple([length] + single_shape)

        array = np.ndarray(count, dtype=dtype)

        max_read_count = format.BUFFER_SIZE // min(format.BUFFER_SIZE, dtype.itemsize)
        for i in range(0, count, max_read_count):
            read_count = min(max_read_count, count - i)
            read_size = int(read_count * dtype.itemsize)
            data = format._read_bytes(fid, read_size, "array data")
            array[i:i+read_count] = np.frombuffer(data, 
                                                dtype=dtype,
                                                count=read_count)
        
        if fortran_order:
            array.shape = shape[::-1]
            array = array.transpose()
        else:
            array.shape = shape

    return array


def load_wav_file(path, start=0, length=-1, return_shape=False):
    with CustomSoundFile(path, 'r') as f:
        if return_shape:
            return (f.frames, f.channels)
        frames = f._prepare_read(start, None, length)
        data = f.read(frames, 'float64', False, None, None)
        return data.astype(np.float32), f.samplerate


class CustomSoundFile(SoundFile):

    def __init__(self, file, mode='r', samplerate=None, channels=None,
                 subtype=None, endian=None, format=None, closefd=True):
       
        # resolve PathLike objects (see PEP519 for details):
        # can be replaced with _os.fspath(file) for Python >= 3.6
        file = file.__fspath__() if hasattr(file, '__fspath__') else file
        self._name = file
        if mode is None:
            mode = getattr(file, 'mode', None)
        mode_int = sf._check_mode(mode)
        self._mode = mode
        self._info = sf._create_info_struct(file, mode, samplerate, channels,
                                         format, subtype, endian)
        self._file = file
        if not isinstance(file, (io.FileIO, io.BufferedReader, io.BufferedWriter)):
            self._file = self._open(file, mode_int, closefd)
        
        if set(mode).issuperset('r+') and self.seekable():
            # Move write position to 0 (like in Python file objects)
            self.seek(0)
        sf._snd.sf_command(self._file, sf._snd.SFC_SET_CLIPPING, sf._ffi.NULL,
                           sf._snd.SF_TRUE)


def to_model(x):
    if isinstance(x, (tuple, list)):
        features = [to_model(x) for x in x]
    elif isinstance(x, dict):
        features = {k: to_model(v) for k, v in x.items()}
    else:
        features = to_gpu(torch.as_tensor(x))
    return features

          
def to_gpu(x):
    x = x.contiguous()
    if torch.cuda.is_available():
        x = x.cuda(non_blocking=True)
    return torch.autograd.Variable(x)


def get_mask_from_lengths(lengths, max_len=None):
    max_len = torch.max(lengths).item() if max_len is None else max_len
    ids = torch.arange(0, max_len).to(lengths.device)
    mask = ~(ids < lengths.unsqueeze(1)).bool()
    return mask


def align_features(feat_dict, fs_dict):
    seq_dict = {k: v for k, v in feat_dict.items()
                if k in fs_dict and fs_dict[k] > 0}
    if len(seq_dict.keys()) == 0:
        return seq_dict

    # Same duration
    durations = {k: 1. * v.shape[0] * fs_dict[k] for k, v in seq_dict.items()}
    
    max_duration = max(durations.values())
    min_duration = min(durations.values())
    if max_duration / min_duration >= 1.1:
        print("files are unaligned seriously: {}".format(durations))
        raise RuntimeError

    align_dict = {k: v[: int(min_duration / fs_dict[k])]
                  for k, v in seq_dict.items()}
    
    # Match the LCM
    fs_lcm = np.lcm.reduce([fs_dict[k] for k in align_dict])
    cliped_duration = min_duration - min_duration % fs_lcm
    align_dict = {k: align_dict[k][: int(cliped_duration / fs_dict[k])]
                  for k in align_dict.keys()}
    feat_dict.update(align_dict)
    return feat_dict


def feature_normalize(feature, config, denormalize=False):
    if denormalize:
        feature = (feature - config['shift']) / config['scale']

    if config['method'] == 'minmax':
        min_vector = np.asarray(config['min'])
        max_vector = np.asarray(config['max'])
        ran_vector = max_vector - min_vector
        feature = (feature - min_vector) / ran_vector \
            if not denormalize else ran_vector * feature + min_vector

    if not denormalize:
        feature = feature * config['scale'] + config['shift']
    
    return feature.astype(np.float32)

'''
Checkpoint Load & Save
'''
def load_checkpoint(checkpoint_object, model, optimizer=None, module=None):
    # Parse checkpoint pbject
    if isinstance(checkpoint_object, (tuple, list)):
        '''
        checkpoint_object can be a list with the format:
        [
            ["acoustic_model.*", "model_path1"],
            ["discriminator.*", "model_path1"],
        ]
        '''
        iteration = 0
        for module, object in checkpoint_object:
            iteration = max(iteration, load_checkpoint(
                object, model, optimizer, module))
        return iteration
    
    if isinstance(checkpoint_object, str):
        assert os.path.isfile(checkpoint_object)
        ckpt_dict = torch.load(checkpoint_object, map_location='cpu')
    elif isinstance(checkpoint_object, dict):
        ckpt_dict = checkpoint_object
    else:
        raise TypeError(f"Unacceptable type: {type(checkpoint_object)}")
    
    ckpt_model_parameters = ckpt_dict['model']
    iteration = ckpt_dict['iteration'] if 'iteration' in ckpt_dict else 0
    
    if module is not None:
        model_parameter_names = model.state_dict().keys()
        ks = [k for k in model_parameter_names if re.match(module, k)]
        model_params = {k: ckpt_model_parameters[k] for k in ks}        
        model.load_state_dict(model_params, strict=False)
    else:
        try:
            model.load_state_dict(ckpt_model_parameters)
            if optimizer is not None:
                ckpt_optim_parameters = ckpt_dict['optimizer']
                optimizer.load_state_dict(ckpt_optim_parameters)
        except:
            print('Loaded model is not the same as the current one')
            model.load_state_dict(ckpt_model_parameters, strict=False)
        
    print("Checkpoint loading is completed.")
    return iteration


def save_checkpoint(checkpoint_dict, filepath,
                    autoclean=False, save_interval=50000):
    if autoclean:
        clean_checkpoint_directory(filepath, save_interval)
    torch.save(checkpoint_dict, filepath)


def clean_checkpoint_directory(checkpoint_path, interval=1000000):
    checkpoint_dir, model_name = os.path.split(checkpoint_path)
    prefix, iterations = model_name.split('_')
    iterations = int(iterations)
    for filename in os.listdir(checkpoint_dir):
        if prefix not in filename:
            continue
        iters = int(filename.split('_')[-1])
        if iters % interval != 0 and iterations - iters > interval:
            print(f"Remove redundant checkpoint: {filename}")
            os.remove(os.path.join(checkpoint_dir, filename))


'''
Module Search
'''
def module_search(names, directory, package=None):
    anchors = [names] if isinstance(names, str) else names
    
    filepaths = glob.glob(os.path.join(directory, "*.py")) + \
                glob.glob(os.path.join(directory, "*", "__init__.py"))
    filepaths = [x[len(directory):] for x in filepaths]
    module_files = [x[: -3].replace(os.path.sep, '.').replace('.__init__', '')
                    for x in filepaths]
    module_files = [x for x in module_files if len(x) > 0]
    
    selected_modules = [None] * len(anchors)

    for i, name in enumerate(anchors):
        class_name = name.split('.')[-1]
        directory = name[: -len(class_name) - 1]
        search_space = module_files
        if directory != '':
            search_space = [package + '.' + directory]
        for module_file in search_space:
            modules = importlib.import_module(module_file, package=package)
            if not hasattr(modules, class_name):
                continue
            module = getattr(modules, class_name)
            path = inspect.getfile(module)

            if selected_modules[i] is not None:
                found_path = inspect.getfile(selected_modules[i])
                if found_path != path:
                    raise RuntimeError("Repeated Module for {}: {}, {}".format(
                        class_name, found_path, path))
                continue
            print('Load {} from file "{}"'.format(class_name, path))
            
            selected_modules[i] = module

    if None in selected_modules:
        raise RuntimeError('Found dismatched modules for {}'.format(names))
    
    if isinstance(names, str):
        selected_modules = selected_modules[0]
    return selected_modules