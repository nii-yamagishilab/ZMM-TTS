import os
import numpy as np
language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
import json

speaker_dict = {}
language_dict_new = {}

with open('Dataset/preprocessed_data/ZMM6/train.txt','r') as f1:
    lines1 = f1.readlines()
    for line in lines1:

      language_id = language_dict[line.split('|')[2]]
      speaker_id = line.split('|')[3]
      base_name = line.split('|')[0]
      print (base_name)
      #f2.write(base_name+'\n')
      speaker_dict[base_name] = speaker_id
      language_dict_new[base_name] = language_id

with open("Dataset/preprocessed_data/ZMM6/language_dict.json", "w") as f:
    f.write(json.dumps(language_dict_new, indent=4))
