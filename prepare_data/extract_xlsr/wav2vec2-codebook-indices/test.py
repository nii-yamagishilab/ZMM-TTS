import os
import numpy as np
code_mini_1 = 1
code_max_1 = 1

code_mini_2 = 1
code_max_2 = 1


data_dir = '/home/others/v-cheng-gong/Low_resources_languages/wav2vec2tf/wav2vec2-codebook-indices/Code_index'
filelist = os.listdir(data_dir)
for code_name in filelist:
    code_index = os.path.join(data_dir,code_name)
    code = np.load(code_index)
    if code_mini_1 > np.min(code[:,0]):
        code_mini_1 = np.min(code[:,0])
    if code_max_1 < np.max(code[:,0]):
        code_max_1 = np.max(code[:,0])

    if code_mini_2 > np.min(code[:,1]):
        code_mini_1 = np.min(code[:,1])

    if code_max_1 < np.max(code[:,1]):
        code_max_2 = np.max(code[:,1])

print (code_mini_1,code_max_1,code_mini_2,code_max_2)
