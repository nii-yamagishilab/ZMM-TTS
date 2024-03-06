# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================

import os

with open('Dataset/preprocessed_data/MM6/train.txt','r') as f1, open('Dataset/preprocessed_data/MM6/train_lists.txt','w') as f2:
      lines = f1.readlines()
      for line in lines:
          f2.write(line.split('|')[0]+'\n')



