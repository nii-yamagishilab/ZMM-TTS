import torchaudio
import os
from speechbrain.pretrained import EncoderClassifier
import numpy as np
from tqdm import *
wavs_dir = 'Dataset/MM6/wavs/'
classifier = EncoderClassifier.from_hparams(source="speechbrain/spkrec-ecapa-voxceleb",run_opts={"device":"cuda"} )
for wavf in tqdm(os.listdir(wavs_dir)):
    if wavf.endswith('wav'):
        signal, fs =torchaudio.load(os.path.join(wavs_dir,wavf))
        #print (fs)
        embeddings = classifier.encode_batch(signal)
        #print (embeddings.shape)
        spk_num = embeddings.squeeze(0).cpu().numpy()
        #print (spk_num.shape)
        #print (spk_num)
        np.save(os.path.join('Dataset/preprocessed_data/ZMM6/SpeakerEmb/',wavf.replace('wav','npy')), spk_num)
