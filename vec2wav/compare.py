import os
import numpy as np

data1 = np.load('/home/others/v-cheng-gong/Low_resources_languages/Base_CodeBook/MSMC-TTS-lj-16k/pred_1_M.npy')
data2 = np.load('/home/others/v-cheng-gong/Low_resources_languages/Base_CodeBook/MSMC-TTS-lj-16k/analysis_synthesis_465000_onestage/npy/LJ023-0035.npy')

print (data1.shape)
print (data2.shape)
