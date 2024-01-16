from __future__ import absolute_import, division, print_function, unicode_literals
import os
import numpy as np
import shutil
import sys

#Note clearners

import re
import os
import json
import json
import torch
from scipy.io.wavfile import write
from meldataset import mel_spectrogram, MAX_WAV_VALUE, load_wav
from tqdm import *

from concurrent.futures import ProcessPoolExecutor
from functools import partial
from multiprocessing import cpu_count


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self

def get_mel(x):
    #print (h.fmax)
    return mel_spectrogram(x, h.n_fft, h.num_mels, h.sampling_rate, h.hop_size, h.win_size, h.fmin, h.fmax)



mel_config = 'Config/config_16k_mel.json'




with open(mel_config) as f:
        data = f.read()

global h
json_config = json.loads(data)
h = AttrDict(json_config)



global device
if torch.cuda.is_available():
        torch.cuda.manual_seed(h.seed)
        device = torch.device('cuda')
else:
        device = torch.device('cpu')





Train_Dir = 'Dataset/MM6/wavs/'


Mel_dir = 'Dataset/preprocessed_data/ZMM6/16K_MEL'
wav_lists = os.listdir(Train_Dir)
print (cpu_count())
for wav_file in tqdm(wav_lists):
    if wav_file.endswith('wav'):
      wav_path = os.path.join(Train_Dir,wav_file)
      wav, sr = load_wav(wav_path)
      wav = wav / MAX_WAV_VALUE
      wav = torch.FloatTensor(wav).to(device)
      x = get_mel(wav.unsqueeze(0))
      #print (base_name)
      x = x.squeeze(0).T
      mel_dir= os.path.join(Mel_dir,wav_file.split('.')[0]+'.npy')
      #print (x.shape)
      np.save(mel_dir,np.array(x.cpu())) 
    
