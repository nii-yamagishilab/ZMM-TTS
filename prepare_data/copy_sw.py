import os
import numpy
import shutil

target_dir = 'Dataset/MM6/wavs/'

with open('Dataset/preprocessed_data/ZMM6/train.txt','r') as f1:
   lines = f1.readlines()
   j = 0
   dur_temp = 0
   for line in lines:
       language  = line.split('|')[2]
       base_name =  line.split('|')[0]
       text = line.split('|')[-1].rstrip()
       #j = 0
       #dur_temp = 0
       if language == 'Swedish':
          shutil.copy(os.path.join('../Model_For_Group1_single_speker_v1/Dataset/Group1S/Train/',base_name+'.wav'),target_dir)
          j = j + 1
#print (j,dur_temp)
