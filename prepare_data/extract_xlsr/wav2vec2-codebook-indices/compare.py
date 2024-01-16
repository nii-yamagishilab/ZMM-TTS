import os
import numpy as np
LDR = np.load('/home/others/v-cheng-gong/Low_resources_languages/Comprehensive-Transformer-TTS-LDR-lst-wo-var/preprocessed_data/LJSpeech/LJ_quantization/LJ001-0001.npy')
index = np.load('/home/others/v-cheng-gong/Low_resources_languages/Comprehensive-Transformer-TTS-LDR-lst-wo-var/preprocessed_data/LJSpeech/Code_index/LJ001-0001.npy')
book1 = np.load('code_book1.npy')
book2 = np.load('code_book2.npy')
print (index.shape)
print (LDR.shape)
code_index1 = index[100][0]
code_index2 = index[100][1]

LDR1 = LDR[100][0:384]
LDR2 = LDR[100][384:]

Look1 = book1[code_index1]
Look2 = book2[code_index2]

assert LDR1.all() == Look1.all()
assert LDR2.all() == Look2.all()
print (code_index1)
print (code_index2)
print (LDR1)
print (LDR2)
print (Look1)
print (Look2)
