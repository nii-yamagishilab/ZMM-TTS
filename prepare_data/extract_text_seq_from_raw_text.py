import torch
#from scipy.io.wavfile import write
from tqdm import *
from concurrent.futures import ProcessPoolExecutor
from functools import partial
from multiprocessing import cpu_count
from transformers import AutoModel, AutoTokenizer
from text2phonemesequence import Text2PhonemeSequence
import torch
import json
import numpy as np
import os
import re
os.environ["TOKENIZERS_PARALLELISM"] = "false"

# Load XPhoneBERT model and its tokenizer
xphonebert = AutoModel.from_pretrained("vinai/xphonebert-base")
tokenizer = AutoTokenizer.from_pretrained("vinai/xphonebert-base")

text2phone_model_eng = Text2PhonemeSequence(language='eng-us', is_cuda=True)
text2phone_model_fra = Text2PhonemeSequence(language='fra', is_cuda=True)
text2phone_model_ger = Text2PhonemeSequence(language='ger', is_cuda=True)
text2phone_model_por = Text2PhonemeSequence(language='por-bz', is_cuda=True)
text2phone_model_spa = Text2PhonemeSequence(language='spa', is_cuda=True)
text2phone_model_swe = Text2PhonemeSequence(language='swe', is_cuda=True)


import pandas as pd
from epitran.backoff import Backoff
import epitran
#backoff = Backoff(['eng-Latn'])
#backoff.transliterate('english')

epi_eng = epitran.Epitran('eng-Latn')
epi_fra = epitran.Epitran('fra-Latn')
epi_ger = epitran.Epitran('deu-Latn')
epi_por = epitran.Epitran('por-Latn')
epi_spa = epitran.Epitran('spa-Latn')
epi_swe = epitran.Epitran('swe-Latn')




def process_text(text,language):
    raw_text = text
    IPA_phones = []
    if language == 'English':
       text1 = raw_text
       input_phonemes = text2phone_model_eng.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_eng.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")


    if language == 'French':
       text1 = raw_text
       text1 = text1.replace('– ','')
       input_phonemes = text2phone_model_fra.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_fra.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")

    if language == 'German':
       text1 = raw_text
       input_phonemes = text2phone_model_ger.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_ger.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")

    if language == 'Portuguese':
       text1 = raw_text
       input_phonemes = text2phone_model_por.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_por.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")

    if language == 'Swedish':
       text1 = raw_text
       input_phonemes = text2phone_model_swe.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_swe.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")

    if language == 'Spanish':
       text1 = raw_text
       input_phonemes = text2phone_model_spa.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_spa.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")

    return input_ids_val, IPA_phones

OOV=[]
lepath = 'Dataset/preprocessed_data/ZMM6/letters_seq'
xppath = 'Dataset/preprocessed_data/ZMM6/xpbid_seq'
ipapath = 'Dataset/preprocessed_data/ZMM6/ipa_seq'
with open("Config/letters_dict_ascill.json", "r") as f, open('Config/ipa_dict_ascill.json','r') as fipa, open('Dataset/preprocessed_data/ZMM6/train.txt','r') as ftrain:
    letter_dict = json.load(f)
    ipa_dict = json.load(fipa)
    lines = ftrain.readlines()
    for line in tqdm(lines):
        text = line.split('|')[-1].rstrip()
        language = line.split('|')[2]
        Xpid, IPA_phones = process_text(text,language)
        print (IPA_phones)
        Xpid = Xpid.cpu().numpy()
        LettersID = []
        IPAID = []
        base_name = line.split('|')[0]
        for character in text:
            if character in letter_dict:
                LettersID.append(letter_dict[character])
            else:
                OOV.append(character)
                print ('----oov-----',character)
        for ipa in IPA_phones:
            if ipa in ipa_dict:
               IPAID.append(ipa_dict[ipa])
            else:
               print ('---oov-----',ipa)
               OOV.append(ipa)
        #np.save(os.path.join(lepath,base_name+'.npy'),np.array(LettersID))
        #np.save(os.path.join(xppath,base_name+'.npy'),Xpid)
        #print (np.array(IPAID))
        np.save(os.path.join(ipapath,base_name+'.npy'),np.array(IPAID))

print ('--oov---')
print (OOV)
        #print (text)
        #print (Xpid)
        #print (LettersID)
