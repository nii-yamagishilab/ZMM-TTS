# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================

#Try to male a speaker-gender-language balance data
#Max 25 speakers per gender per language
#For training speed, each audio piece should not exceed 15 seconds at most.
#On average, each speaker has about 200 audio clips
#For English, German, French, Spanish

import soundfile as sf 
import os 
import shutil
summary = []
#for 
#for language in ['english','portuguese']:

ftrain = open('Dataset/preprocessed_data/ZMM6/train.txt','w')
target_wavs_path = 'Dataset/MM6/wavs/'
for language in ['english','french', 'german', 'portuguese','spanish']:
  with open (os.path.join('../Dataset/Multilingual_LibriSpeech_fl','mls_'+language, 'train', 'transcripts.txt'), 'r') as f2:
        #print (language) 
        line_texts = f2.readlines()
        language_text_dict={}
        for line_text in line_texts:
            #print (line_text.split('\t')[1])
            language_text_dict[line_text.split('\t')[0]] = line_text.split('\t')[1].strip()
            #print (line_text.split('\t')[0])
            #print (line_text.split('\t')[1])
  with open (os.path.join('../Dataset/Multilingual_LibriSpeech_fl','mls_'+language, 'metainfo.txt'), 'r') as f1:
        print (language)
        lines=f1.readlines()
        speaker_dur_dict_f={}
        speaker_dur_dict_m={}
        for line in lines[1:]:
           line = line[:-1].replace(' ','')
           parts=line.split('|')
           if parts[2]=='train' and  parts[1]=='F':
             if parts[0] not in speaker_dur_dict_f:
                speaker_dur_dict_f[parts[0]] = 0.0
             speaker_dur_dict_f[parts[0]] = speaker_dur_dict_f[parts[0]] + float(parts[3])
           if parts[2]=='train' and  parts[1]=='M':
             if parts[0] not in speaker_dur_dict_m:
                speaker_dur_dict_m[parts[0]] = 0.0
             speaker_dur_dict_m[parts[0]] = speaker_dur_dict_m[parts[0]] + float(parts[3])

        #print (speaker_dur_dict_f)
        sorted_dict = sorted(speaker_dur_dict_f.items(), key=lambda x: x[1],reverse=True)
        #print(sorted_dict)
        meta_numbers = []
        meta_dur = []
        for speaker in sorted_dict[:20]:
            #print ('female',speaker[0],speaker[1])
            #if 15*200 < speaker[1]*60:
                #print ('female',speaker[0],speaker[1])
                
                if language == 'portuguese':
                   max_num = 2000
                else:
                   max_num = 200
                
                files=os.listdir('../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0])
                num = 0
                dur_temp = 0 
                wav_number = 0 
                for book in files:
                    num_temp = os.listdir('../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0]+'/'+book)
                    #print ('num_temp',num_temp)
                    for wav_file in num_temp:
                        #print (wav_file)
                        if wav_file.endswith('flac') and wav_number<max_num:
                          audio_path = '../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0]+'/'+book+'/'+ wav_file
                          data, sr = sf.read(audio_path)
                          #print (data)
                          #print (len(data))
                          dur_length = len(data)/sr
                          #print (audio_path)
                          text = language_text_dict[wav_file.split('.')[0]]
                          #print (dur_length)
                          if dur_length >3 and dur_length < 15:
                               current = [language.capitalize(),'female',dur_length,speaker[0],'MLS',wav_file,text]
                               #print (current)
                               shutil.copy(audio_path,target_wavs_path)
                               ftrain.write(wav_file.split('.')[0]+'|ZMM6|'+language.capitalize()+'|'+speaker[0]+'|'+text+'\n')
                               dur_temp = dur_temp + dur_length
                               summary.append(current)
                               wav_number = wav_number + 1
                meta_numbers.append(wav_number)
                meta_dur.append(dur_temp)
                #print ('female',speaker[0],dur_temp,wav_number)
        assert len(meta_numbers) == len(meta_dur)
        print ('female speakers:', len(meta_numbers))
        print ('total sens',sum(meta_numbers))
        print ('total durs', sum(meta_dur))
        sorted_dict = sorted(speaker_dur_dict_m.items(), key=lambda x: x[1],reverse=True)
        meta_numbers = []
        meta_dur = []
        #print(sorted_dict)
        for speaker in sorted_dict[:20]:
            #print (speaker[0],speaker[1])
                files=os.listdir('../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0])
                num = 0
                dur_temp = 0 
                wav_number = 0                
                if language == 'portuguese':
                   max_num = 1000
                else:
                   max_num = 200
                for book in files:
                    num_temp = os.listdir('../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0]+'/'+book)
                    for wav_file in num_temp:
                        if wav_file.endswith('flac') and wav_number<max_num:
                          audio_path = '../Dataset/Multilingual_LibriSpeech_fl/'+'mls_'+language+'/train/audio/'+ speaker[0]+'/'+book+'/'+ wav_file
                          data, sr = sf.read(audio_path)
                          dur_length = len(data)/sr
                          text = language_text_dict[wav_file.split('.')[0]]
                          if dur_length >3 and dur_length < 15:
                                 current = [language.capitalize(),'male',dur_length,speaker[0],'MLS',wav_file,text]
                                 dur_temp = dur_temp + dur_length
                                 shutil.copy(audio_path,target_wavs_path)
                                 ftrain.write(wav_file.split('.')[0]+'|ZMM6|'+language.capitalize()+'|'+speaker[0]+'|'+text+'\n')
                                 summary.append(current)
                                 wav_number = wav_number + 1
                meta_numbers.append(wav_number)
                meta_dur.append(dur_temp)
                #print ('male',speaker[0],dur_temp,wav_number)
        assert len(meta_numbers) == len(meta_dur)
        print ('male speakers:', len(meta_numbers))
        print ('total sens',sum(meta_numbers))
        print ('total durs', sum(meta_dur))

print('---swedish----')

with open('../Model_For_Group1_single_speker_v1/Dataset/preprocessed_data/Group1S/train_unsup.txt','r') as f1:
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
          data, sr = sf.read(os.path.join('../Model_For_Group1_single_speker_v1/Dataset/Group1S/Train/',base_name+'.wav'))
          dur_length = len(data)/sr
          dur_temp = dur_temp + dur_length
          ftrain.write(base_name+'|ZMM6|'+language.capitalize()+'|swnht0|'+text+'\n')
          j = j + 1
print (j,dur_temp)
