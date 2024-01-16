import os
import numpy as np
from scipy.stats import betabinom

from concurrent.futures import ProcessPoolExecutor
from functools import partial
from multiprocessing import cpu_count


executor = ProcessPoolExecutor(max_workers=cpu_count())

def beta_binomial_prior_distribution(phoneme_count, mel_count, scaling_factor=1.0):
     P, M = phoneme_count, mel_count
     x = np.arange(0, P)
     mel_text_probs = []
     for i in range(1, M+1):
        a, b = scaling_factor*i, scaling_factor*(M+1-i)
        rv = betabinom(P, a, b)
        mel_i_prob = rv.pmf(x)
        mel_text_probs.append(mel_i_prob)
     return np.array(mel_text_probs)

def save_npy(target=None,source=None,base_name=None):
    #attn_prior_xp_dr  = beta_binomial_prior_distribution(target[0],source[0],1.0)
    #attn_prior_le_dr  = beta_binomial_prior_distribution(target[1],source[1],1.0)
    #attn_prior_dr_mel = beta_binomial_prior_distribution(target[2],source[2],1.0)
    attn_prior_ipa_dr = beta_binomial_prior_distribution(target[3],source[3],1.0)
    attn_prior_filename = "{}-attn_prior-{}".format('MM6', base_name)
    #np.save(os.path.join("Dataset/preprocessed_data/ZMM6/xp_dr_prior", attn_prior_filename), attn_prior_xp_dr)
    #np.save(os.path.join("Dataset/preprocessed_data/ZMM6/le_dr_prior",  attn_prior_filename), attn_prior_le_dr)
    #np.save(os.path.join("Dataset/preprocessed_data/ZMM6/dr_mel_prior", attn_prior_filename), attn_prior_dr_mel)
    np.save(os.path.join("Dataset/preprocessed_data/ZMM6/ipa_dr_prior", attn_prior_filename), attn_prior_ipa_dr)
Mel_dir = 'Dataset/preprocessed_data/ZMM6/16K_MEL/'
Le_dir = 'Dataset/preprocessed_data/ZMM6/letters_seq/'
Dr_dir = 'Dataset/preprocessed_data/ZMM6/DiscreteRp/'
Xp_dir = 'Dataset/preprocessed_data/ZMM6/xpbid_seq/' 
ipa_dir = 'Dataset/preprocessed_data/ZMM6/ipa_seq/' 
lines = os.listdir(Mel_dir)
for line in lines:
       #target = []
       #soure = []
       base_name = line.split('.')[0]
       mel = np.load(os.path.join(Mel_dir,line))
       letter = np.load(os.path.join(Le_dir,line))
       dr = np.load(os.path.join(Dr_dir,line))
       xp = np.load(os.path.join(Xp_dir,line))
       ipa = np.load(os.path.join(ipa_dir,line))
       target = [dr.shape[0],dr.shape[0],mel.shape[0],dr.shape[0]]
       source = [xp.shape[1],letter.shape[0],dr.shape[0],ipa.shape[0]]
       
       #print (target)
       #print (source)
        
       executor.submit(partial(save_npy, target, source, base_name))
