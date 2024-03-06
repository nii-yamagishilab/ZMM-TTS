# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================

import soundfile as sf
import os
import shutil
summary = []
#for
#for language in ['english','portuguese']:

ftrain = open('Dataset/preprocessed_data/MM6/train.txt','a+')
target_wavs_path = 'Dataset/MM6/wavs/'
print('---swedish----')
with open('Dataset/origin_data/train_unsup_swe.txt','r') as f1:
   lines = f1.readlines()
   j = 0
   dur_temp = 0
   for line in lines:
       language  = line.split('|')[2]
       base_name =  line.split('|')[0]
       text = line.split('|')[-1].rstrip()
       #j = 0
       #dur_temp = 0
       if language == 'swedish':
          data, sr = sf.read(os.path.join('Dataset/origin_data/NHTSwedish',base_name+'.wav'))
          dur_length = len(data)/sr
          dur_temp = dur_temp + dur_length
          ftrain.write(base_name+'|MM6|'+language.capitalize()+'|swnht0|'+text+'\n')
          j = j + 1
print (j,dur_temp)
