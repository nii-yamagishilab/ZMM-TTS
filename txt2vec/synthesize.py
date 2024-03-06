import re
import os
import json
import argparse
from string import punctuation
import numpy

import torch
import yaml
import numpy as np
from torch.utils.data import DataLoader
from g2p_en import G2p
from pypinyin import pinyin, Style

from utils.model import get_model, get_vocoder
from utils.tools import get_configs_of, to_device, synth_samples,  get_configs_of
from dataset import TextDataset
from text import text_to_sequence
import torchaudio
import os
from speechbrain.pretrained import EncoderClassifier
from transformers import AutoModel, AutoTokenizer
from text2phonemesequence import Text2PhonemeSequence
from epitran.backoff import Backoff
import epitran
#backoff = Backoff(['eng-Latn'])
#backoff.transliterate('english')
os.environ["TOKENIZERS_PARALLELISM"] = "false"


epi_eng = epitran.Epitran('eng-Latn')
epi_fra = epitran.Epitran('fra-Latn')
epi_ger = epitran.Epitran('deu-Latn')
epi_por = epitran.Epitran('por-Latn')
epi_spa = epitran.Epitran('spa-Latn')
epi_swe = epitran.Epitran('swe-Latn')



def read_lexicon(lex_path):
    lexicon = {}
    with open(os.path.join(lex_path)) as f:
        for line in f:
            temp = re.split(r"\s+", line.strip("\n"))
            word = temp[0]
            phones = temp[1:]
            if word.lower() not in lexicon:
                lexicon[word.lower()] = phones
    return lexicon


def preprocess_english(text, preprocess_config):
    '''
    text = text.rstrip(punctuation)
    lexicon = read_lexicon(preprocess_config["path"]["lexicon_path"])

    g2p = G2p()
    phones = []
    words = re.split(r"([,;.\-\?\!\s+])", text)
    for w in words:
        if w.lower() in lexicon:
            phones += lexicon[w.lower()]
        else:
            phones += list(filter(lambda p: p != " ", g2p(w)))
    phones = "{" + "}{".join(phones) + "}"
    phones = re.sub(r"\{[^\w\s]?\}", "{sp}", phones)
    phones = phones.replace("}{", " ")

    print("Raw Text Sequence: {}".format(text))
    print("Phoneme Sequence: {}".format(phones))
    '''
    sequence = np.array(
        text_to_sequence(
            text, preprocess_config["preprocessing"]["text"]["text_cleaners"]
        )
    )

    return np.array(sequence)


def preprocess_mandarin(text, preprocess_config):
    lexicon = read_lexicon(preprocess_config["path"]["lexicon_path"])

    phones = []
    pinyins = [
        p[0]
        for p in pinyin(
            text, style=Style.TONE3, strict=False, neutral_tone_with_five=True
        )
    ]
    for p in pinyins:
        if p in lexicon:
            phones += lexicon[p]
        else:
            phones.append("sp")

    phones = "{" + " ".join(phones) + "}"
    print("Raw Text Sequence: {}".format(text))
    print("Phoneme Sequence: {}".format(phones))
    sequence = np.array(
        text_to_sequence(
            phones, preprocess_config["preprocessing"]["text"]["text_cleaners"]
        )
    )

    return np.array(sequence)


def synthesize(device, model, args, configs, batchs, control_values,output_dir):
    preprocess_config, model_config, train_config = configs
    pitch_control, energy_control, duration_control = control_values

    for batch in batchs:

        batch = to_device(batch, device)
        basename = batch[0] 
        with torch.no_grad():
            # Forward
            output = model(
                *(batch[2:-1]),
                language_id = batch[-1],
                p_control=pitch_control,
                e_control=energy_control,
                d_control=duration_control
            )
            #print ('---postnet output----',output[1].shape)
            
            pred1 = torch.nn.functional.softmax(output[0][:,:,0:320],dim=-1)
            pred2 = torch.nn.functional.softmax(output[0][:,:,320:],dim=-1 )
            pred1 = torch.argmax(pred1,dim=-1)
            pred2 = torch.argmax(pred2,dim=-1)
            #print (pred1.shape)
            #print (pred2.shape)
            pred =  torch.cat([pred1.unsqueeze(-1),pred2.unsqueeze(-1)],-1)
            #print (pred,pred.shape)
            #np.save('pred_3.npy',np.array(pred.squeeze(0).cpu()))
            #print (torch.nn.functional.softmax(output[1][:,:,0:320]).shape)
            #print (torch.softmax(output[0][0][0]))
            
            np.save(os.path.join(output_dir, basename+'.npy'),np.array(pred.squeeze(0).cpu()))
            book1 = np.load('Dataset/MM6/code_book1.npy')
            book2 = np.load('Dataset/MM6/code_book2.npy')
            code_embed = np.empty((pred.squeeze(0).shape[0],768))
            #code_embed_2 = numpy.empty((code.shape(0),768))
            i = 0
            for code in np.array(pred.squeeze(0).cpu()):
                code_embed[i,0:384] = book1[code[0]]
                code_embed[i,384:768] = book2[code[1]]
                i = i+1
            #print (code_embed.dtype) 
            np.save(os.path.join(output_dir+'_conver', basename+'.npy'),code_embed.astype(np.float32))
            
            '''
            synth_samples(
                batch,
                output,
                vocoder,
                model_config,
                preprocess_config,
                train_config["path"]["result_path"],
                args,
            )
            '''


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--restore_step", type=int, required=True)
    parser.add_argument(
        "--mode",
        type=str,
        choices=["batch", "single", "test","test_multi","test_multi_asr","test_multi_unseen","quick_test"],
        required=True,
        help="Synthesize a whole dataset or a single sentence",
    )
    parser.add_argument(
        "--source",
        type=str,
        default=None,
        help="path to a source file with format like train.txt and val.txt, for batch mode only",
    )
    parser.add_argument(
        "--text",
        type=str,
        default=None,
        help="raw text to synthesize, for single-sentence mode only",
    )
    parser.add_argument(
        "--speaker_id",
        type=str,
        default="p225",
        help="speaker ID for multi-speaker synthesis, for single-sentence mode only",
    )
    parser.add_argument(
        "--dataset",
        type=str,
        required=True,
        help="name of dataset",
    )
    
    parser.add_argument(
        "--config",
        type=str,
        required=True,
        help="name of config",
    )

     
    parser.add_argument(
        "--input_textlist",
        type=str,
        default=None,
        help="input list",
    )
    
    parser.add_argument(
        "--output_dir",
        type=str,
        default=None,
        help="input list",
    )

    
    parser.add_argument(
        "--pretrained_model",
        type=str,
        default=None,
        help="use our paper's model",
    )
 
    parser.add_argument(
        "--pitch_control",
        type=float,
        default=1.0,
        help="control the pitch of the whole utterance, larger value for higher pitch",
    )
    parser.add_argument(
        "--energy_control",
        type=float,
        default=1.0,
        help="control the energy of the whole utterance, larger value for larger volume",
    )
    parser.add_argument(
        "--duration_control",
        type=float,
        default=1.0,
        help="control the speed of the whole utterance, larger value for slower speaking rate",
    )
    args = parser.parse_args()

    # Check source texts
    if args.mode == "batch":
        assert args.source is not None and args.text is None
    if args.mode == "single":
        assert args.source is None and args.text is not None

    # Read Config
    preprocess_config, model_config, train_config = get_configs_of(args.config)
    configs = (preprocess_config, model_config, train_config)
    if preprocess_config["preprocessing"]["pitch"]["pitch_type"] == "cwt":
        from utils.pitch_tools import get_lf0_cwt
        preprocess_config["preprocessing"]["pitch"]["cwt_scales"] = get_lf0_cwt(np.ones(10))[1]
    os.makedirs(
        os.path.join(train_config["path"]["result_path"], str(args.restore_step)), exist_ok=True)

    # Set Device
    torch.manual_seed(train_config["seed"])
    if torch.cuda.is_available():
        torch.cuda.manual_seed(train_config["seed"])
        device = torch.device('cuda')
    else:
        device = torch.device('cpu')
    print("Device of CompTransTTS:", device)

    # Get model
    model = get_model(args, configs, device, train=False)

    #classifier = EncoderClassifier.from_hparams(source="speechbrain/spkrec-ecapa-voxceleb",run_opts={"device":"cuda"} )
    xphonebert = AutoModel.from_pretrained("vinai/xphonebert-base")
    tokenizer = AutoTokenizer.from_pretrained("vinai/xphonebert-base")

    text2phone_model_eng = Text2PhonemeSequence(language='eng-us', is_cuda=True)
    text2phone_model_fra = Text2PhonemeSequence(language='fra', is_cuda=True)
    text2phone_model_ger = Text2PhonemeSequence(language='ger', is_cuda=True)
    text2phone_model_por = Text2PhonemeSequence(language='por-bz', is_cuda=True)
    text2phone_model_spa = Text2PhonemeSequence(language='spa', is_cuda=True)
    text2phone_model_swe = Text2PhonemeSequence(language='swe', is_cuda=True) 
    
    def process_text(text,language):
     raw_text = text
     IPA_phones = []
     if language == 'English':
       text1 = raw_text
       input_phonemes = text2phone_model_eng.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_eng.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''

     if language == 'French':
       text1 = raw_text
       text1 = text1.replace('– ','')
       input_phonemes = text2phone_model_fra.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_fra.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''
     if language == 'German':
       text1 = raw_text
       input_phonemes = text2phone_model_ger.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_ger.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''

     if language == 'Portuguese':
       text1 = raw_text
       input_phonemes = text2phone_model_por.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_por.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''
     if language == 'Swedish':
       text1 = raw_text
       input_phonemes = text2phone_model_swe.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_swe.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''

     if language == 'Spanish':
       text1 = raw_text
       input_phonemes = text2phone_model_spa.infer_sentence(text1)
       input_ids = tokenizer(input_phonemes, return_tensors="pt")
       input_ids_val = input_ids['input_ids']
       
       '''
       words = filter(None, re.split(r"([,;.\-\?\!\s+])", text1))
       for w in words:
           IPA_phones +=   list(filter(lambda p: p != " ", epi_spa.transliterate(w)))
           if w == " ":
              IPA_phones = IPA_phones + list(" ")
       '''

     return input_ids_val, IPA_phones     
    
    # Load vocoder
    #vocoder = get_vocoder(model_config, device)

    # Preprocess texts
    if args.mode == "batch":
        # Get dataset
        dataset = TextDataset(args.source, preprocess_config, model_config)
        batchs = DataLoader(
            dataset,
            batch_size=8,
            collate_fn=dataset.collate_fn,
        )
    if args.mode == "single":
        ids = raw_texts = [args.text[:100]]

        # Speaker Info
        load_spker_embed = model_config["multi_speaker"] \
            and preprocess_config["preprocessing"]["speaker_embedder"] != 'none'
        with open(os.path.join(preprocess_config["path"]["preprocessed_path"], "speakers.json")) as f:
            speaker_map = json.load(f)
        speakers = np.array([speaker_map[args.speaker_id]]) if model_config["multi_speaker"] else np.array([0]) # single speaker is allocated 0
        spker_embed = np.load(os.path.join(
            preprocess_config["path"]["preprocessed_path"],
            "spker_embed",
            "{}-spker_embed.npy".format(args.speaker_id),
        )) if load_spker_embed else None

        if preprocess_config["preprocessing"]["text"]["language"] == "en":
            texts = np.array([preprocess_english(args.text, preprocess_config)])
        elif preprocess_config["preprocessing"]["text"]["language"] == "zh":
            texts = np.array([preprocess_mandarin(args.text, preprocess_config)])
        text_lens = np.array([len(texts[0])])
        batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), spker_embed)]
    if args.mode == "test":
        filelists = open(args.input_textlist,'r').readlines()
        i = 0
        os.makedirs(args.output_dir, exist_ok=True)
        os.makedirs(args.output_dir+'_conver', exist_ok=True)
        for cur_text in filelists:
            ids = raw_texts = 'pred_mel_'+str(i)
                # Speaker Info
            load_spker_embed = model_config["multi_speaker"] \
                  and preprocess_config["preprocessing"]["speaker_embedder"] != 'none'
            with open(os.path.join(preprocess_config["path"]["preprocessed_path"], "speakers.json")) as f:
                 speaker_map = json.load(f)
            speakers = np.array([speaker_map[args.speaker_id]]) if model_config["multi_speaker"] else np.array([0]) # single speaker is allocated 0
            spker_embed = np.load(os.path.join(
                 preprocess_config["path"]["preprocessed_path"],
                 "spker_embed",
                 "{}-spker_embed.npy".format(args.speaker_id),
            )) if load_spker_embed else None 

            
            if preprocess_config["preprocessing"]["text"]["language"] == "en":
                texts = np.array([preprocess_english(cur_text, preprocess_config)])
            elif preprocess_config["preprocessing"]["text"]["language"] == "zh":
                texts = np.array([preprocess_mandarin(cur_text, preprocess_config)])
            text_lens = np.array([len(texts[0])])
            i=i+1
            batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), spker_embed)]
            control_values = args.pitch_control, args.energy_control, args.duration_control

            synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_dir)
    if args.mode == "test_multi":
          filelists = open(args.input_textlist,'r').readlines()
          i = 0
          language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
          os.makedirs(args.output_dir, exist_ok=True)
          seq_file = 'Dataset/preprocessed_data/Group1v2/xpbid_seq'
          os.makedirs(args.output_dir, exist_ok=True)
          os.makedirs(args.output_dir+'_conver', exist_ok=True)
          for cur_text in filelists:
            print (cur_text)
            ids = raw_texts = 'pred_mel_'+str(i)
            base_name = cur_text.split('|')[0]
            #speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',cur_text.split('|')[0].rstrip()+'.npy')
            if len(cur_text.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',cur_text.split('|')[-1].rstrip()+'.npy')
            else:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',cur_text.split('|')[0].rstrip()+'.npy') 
            speakers =  np.load(speaker_file)
            print (speakers)
            print(cur_text.split('|')[2])
            language_id = language_dict[cur_text.split('|')[2]]
            # Speaker Info

            print (language_id)
            print (speakers)

            texts = np.array([np.load(os.path.join(seq_file,base_name+'.npy')).tolist()])
            print (texts.shape)
            text_lens = np.array([texts.shape[1]])
            speakers = np.array([speakers])
            language_id = np.array([language_id])
            i=i+1
            batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
            control_values = args.pitch_control, args.energy_control, args.duration_control

            synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_dir)

    if args.mode == "test_multi_asr":
          filelists = open(args.input_textlist,'r').readlines()
          i = 0
          language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
          os.makedirs(args.output_dir, exist_ok=True)
          seq_file = 'Dataset/test_data/ASR_1000_seq/xpid'
          os.makedirs(args.output_dir, exist_ok=True)
          os.makedirs(args.output_dir+'_conver', exist_ok=True)
          for cur_text in filelists:
            print (cur_text)
            ids = raw_texts = 'pred_mel_'+str(i)
            base_name = cur_text.split('|')[0]
            #speakers =  int(cur_text.split('|')[3])
            
            #print (speakers)
            #print(cur_text.split('|')[2])
            if len(cur_text.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',cur_text.split('|')[-1].rstrip()+'.npy')
            else:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',code_file) 
            language_id = language_dict[cur_text.split('|')[2]]
            # Speaker Info
            speakers = np.load(speaker_file)
            print (language_id)
            print (speakers)

            texts = np.array([np.load(os.path.join(seq_file,base_name+'.npy')).tolist()])
            print (texts.shape)
            text_lens = np.array([texts.shape[1]])
            speakers = np.array([speakers])
            language_id = np.array([language_id])
            i=i+1
            batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
            control_values = args.pitch_control, args.energy_control, args.duration_control

            synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_dir)
     
    if args.mode == "test_multi_unseen":
          filelists = open(args.input_textlist,'r').readlines()
          i = 0
          language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
          os.makedirs(args.output_dir, exist_ok=True)
          seq_file = 'Dataset/preprocessed_data/Group1v2/xpbid_seq'
          os.makedirs(args.output_dir, exist_ok=True)
          os.makedirs(args.output_dir+'_conver', exist_ok=True)
          for cur_text in filelists:
            print (cur_text)
            ids = raw_texts = 'pred_mel_'+str(i)
            base_name = cur_text.split('|')[0]
            #speakers =  int(cur_text.split('|')[3])

            #print (speakers)
            #print(cur_text.split('|')[2])
            if len(cur_text.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/test_data/unseen_audio_spkemb/',cur_text.split('|')[-1].rstrip()+'.npy')
            else:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',code_file)
            language_id = language_dict[cur_text.split('|')[2]]
            # Speaker Info
            print (speaker_file)
            speakers = np.load(speaker_file)
            print (language_id)
            print (speakers)

            texts = np.array([np.load(os.path.join(seq_file,base_name+'.npy')).tolist()])
            print (texts.shape)
            text_lens = np.array([texts.shape[1]])
            speakers = np.array([speakers])
            language_id = np.array([language_id])
            i=i+1
            batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
            control_values = args.pitch_control, args.energy_control, args.duration_control

            synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_dir)
   
    if args.mode == "quick_test":
          filelists = open(args.input_textlist,'r').readlines()
          i = 0
          language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
          os.makedirs(args.output_dir, exist_ok=True)
          #seq_file = 'Dataset/preprocessed_data/Group1v2/xpbid_seq'
          os.makedirs(args.output_dir, exist_ok=True)
          os.makedirs(args.output_dir+'_conver', exist_ok=True)
          for cur_text in filelists:
            print (cur_text)
            ids = raw_texts = 'pred_mel_'+str(i)
            base_name = cur_text.split('|')[0]
            text = cur_text.split('|')[-1].rstrip()
            speaker_file =  os.path.join('Dataset/MM6/test_spk_emb/',base_name+'.npy')
           
            speakers = np.load(speaker_file)
             
            language_id = language_dict[cur_text.split('|')[2]]

            if args.config == 'MM6_XphoneBERT': 
                Xpid, IPA_phones = process_text(text,cur_text.split('|')[2]) 
                texts = np.array([Xpid.cpu().numpy().tolist()])
                text_lens = np.array([texts.shape[1]])
            if args.config == 'MM6_Letters':
               LettersID = [] 
               with open("Config/letters_dict_ascill.json", "r") as f:
                  letter_dict = json.load(f)
                  for character in text:
                      if character in letter_dict:
                         LettersID.append(letter_dict[character])
                      else:
                         print ('----oov-----',character) 
                  texts = np.array([LettersID])
                  print ('--texts----',texts.shape,texts)
                  text_lens = np.array([texts.shape[1]])
            speakers = np.array([speakers])
            language_id = np.array([language_id])
            i=i+1
            batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
            control_values = args.pitch_control, args.energy_control, args.duration_control

            synthesize(device, model, args, configs, batchs, control_values, args.output_dir) 
