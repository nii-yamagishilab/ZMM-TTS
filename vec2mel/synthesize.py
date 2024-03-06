import re
import os
import json
import argparse
from string import punctuation

import torch
import yaml
import numpy as np
from torch.utils.data import DataLoader
from g2p_en import G2p
from pypinyin import pinyin, Style

from utils.model import get_model, get_vocoder
from utils.tools import get_configs_of, to_device, synth_samples
from dataset import TextDataset
from text import text_to_sequence
import numpy 

"""
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
    text = text.rstrip(punctuation)
    lexicon = read_lexicon(os.path.join(preprocess_config["path"]["lexicon_path"]))

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
    sequence = np.array(
        text_to_sequence(
            phones, preprocess_config["preprocessing"]["text"]["text_cleaners"]
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

"""
def synthesize(device, model, args, configs,  batchs, control_values, output_path):
    preprocess_config, model_config, train_config = configs
    pitch_control, energy_control, duration_control = control_values
    #basename = batch[0]
    for batch in batchs:
        batch = to_device(batch, device)
        #print (batch)
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

            np.save(os.path.join(output_path, basename+'.npy'),np.array(output[1].squeeze(0).cpu()))
            
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
        choices=["batch", "single", "test", "test-code","test_multi","test_multi_unseen","quick_test"],
        required=True,
        help="Synthesize a whole dataset or a single sentence",
    )

    parser.add_argument(
        "--code_path",
        type=str,
        default=None,
        help="path to a source file with format like train.txt and val.txt, for batch mode only",
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
        help="name of dataset",
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
    parser.add_argument(
        "--input_path",
        type=str,
        default=None,
        help="path",
    )
    parser.add_argument(
        "--output_path",
        type=str,
        default=None,
        help="path",
    )
    parser.add_argument(
        "--pretrained_model",
        type=str,
        default=None,
        help="use our paper's model",
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
        #text_lens = np.array([len(texts[0])])
        texts = np.array([np.load('/home/others/v-cheng-gong/Low_resources_languages/Comprehensive-Transformer-TTS-LDR-lst-wo-var/preprocessed_data/LJSpeech/Code_index/LJ001-0002.npy')])
        
        #texts = np.array([np.load('pred_3.npy')])
        #texts = texts
        print ('----texts------',texts)     
        print ('----texts------',texts.shape)
        text_lens = np.array([texts.shape[1]])
        print ('-----text_lens-----',text_lens)
        batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens),spker_embed)]

    
    if args.mode == "test":
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
        #text_lens = np.array([len(texts[0])])
        file_lists = os.listdir(args.input_path)
        i = 0
        os.makedirs(args.output_path, exist_ok=True)
        for code_file in file_lists:
             print (code_file)
             #texts = np.array([np.load(os.path.join(args.input_path,code_file))])

             base_name = 'pred_mel_'+str(i)+'.npy'
             texts = np.array([np.load(os.path.join(args.input_path,base_name))])
             #texts = np.array([np.load('pred_3.npy')])
             #texts = texts
             print ('----texts------',texts)
             print ('----texts------',texts.shape)
             text_lens = np.array([texts.shape[1]])
             print ('-----text_lens-----',text_lens)
             ids = 'pred_mel_'+str(i)
             batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens),spker_embed)]
             i = i +1

             control_values = args.pitch_control, args.energy_control, args.duration_control

             synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_path)

    if args.mode == "test-code":
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
        #text_lens = np.array([len(texts[0])])
        file_lists = os.listdir(args.input_path)
        j = 0
        os.makedirs(args.output_path, exist_ok=True)
        for code_file in file_lists:
             print (code_file)
             codes = np.load(os.path.join(args.input_path,code_file))
             print (codes.shape)
             book1 = np.load('Dataset/preprocessed_data/Group1/code_book1.npy')
             book2 = np.load('Dataset/preprocessed_data/Group1/code_book2.npy')  
             code_embed = np.empty((codes.shape[0],768)) 
             #code_embed_2 = numpy.empty((code.shape(0),768))
             i = 0
             for code in codes:
                 code_embed[i,0:384] = book1[code[0]]
                 code_embed[i,384:768] = book2[code[1]]
                 i = i+1
             #texts = np.array([np.load(os.path.join(args.input_path,code_file))])
            

             texts = np.array([code_embed])
             #texts = texts
             print ('----texts------',texts)
             print ('----texts------',texts.shape)
             text_lens = np.array([texts.shape[1]])
             print ('-----text_lens-----',text_lens)
             ids = 'pred_mel_'+str(j)
             batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens),spker_embed)]
             j = j +1

             control_values = args.pitch_control, args.energy_control, args.duration_control

             synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_path)
    if args.mode == "test_multi":
        ids = raw_texts = [args.text[:100]]
        load_spker_embed = model_config["multi_speaker"] \
            and preprocess_config["preprocessing"]["speaker_embedder"] != 'none'
        '''
        with open(os.path.join(preprocess_config["path"]["preprocessed_path"], "speakers.json")) as f:
            speaker_map = json.load(f)
        speakers = np.array([speaker_map[args.speaker_id]]) if model_config["multi_speaker"] else np.array([0]) # single speaker is allocated 0
        '''
        spker_embed = np.load(os.path.join(
            preprocess_config["path"]["preprocessed_path"],
            "spker_embed",
            "{}-spker_embed.npy".format(args.speaker_id),
        )) if load_spker_embed else None

        if preprocess_config["preprocessing"]["text"]["language"] == "en":
            texts = np.array([preprocess_english(args.text, preprocess_config)])
        elif preprocess_config["preprocessing"]["text"]["language"] == "zh":
            texts = np.array([preprocess_mandarin(args.text, preprocess_config)])
        #text_lens = np.array([len(texts[0])])
        #file_lists = os.listdir(args.input_path)
        j = 0
        os.makedirs(args.output_path, exist_ok=True)
        language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5} 
        with open (args.input_path,'r') as f1:
          #f1.readlines()
          for line in f1.readlines():
             code_file = line.split('|')[0]+'.npy'
             
             
             if len(line.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',line.split('|')[-1].rstrip()+'.npy')
             else:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',code_file)
             speakers = np.load(speaker_file) 
             #int(line.split('|')[3])
             print (speakers)
             print(line.split('|')[2])
             language_id = language_dict[line.split('|')[2]]
             print (code_file)
             if args.code_path == None:
               codes = np.load(os.path.join('Dataset/preprocessed_data/Group1v2/CodeIndex/',code_file))
             else:
               print (os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
               codes = np.load(os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
             print (codes.shape)
             book1 = np.load('Dataset/Group1v2/code_book1.npy')
             book2 = np.load('Dataset/Group1v2/code_book2.npy')
             code_embed = np.empty((codes.shape[0],768))
             #code_embed_2 = numpy.empty((code.shape(0),768))
             i = 0
             print (codes)
             for code in codes:
                 code_embed[i,0:384] = book1[code[0]]
                 code_embed[i,384:768] = book2[code[1]]
                 i = i+1
             #texts = np.array([np.load(os.path.join(args.input_path,code_file))])

            
             texts = np.array([code_embed])
             #texts = texts
             print ('----texts------',texts)
             print ('----texts------',texts.shape)
             text_lens = np.array([texts.shape[1]])
             print ('-----text_lens-----',text_lens)
             ids = 'pred_mel_'+str(j)
             speakers = np.array([speakers])
             language_id = np.array([language_id])

             batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
             j = j +1

             control_values = args.pitch_control, args.energy_control, args.duration_control

             synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_path)




    if args.mode == "test_multi_unseen":
        ids = raw_texts = [args.text[:100]]
        load_spker_embed = model_config["multi_speaker"] \
            and preprocess_config["preprocessing"]["speaker_embedder"] != 'none'
        '''
        with open(os.path.join(preprocess_config["path"]["preprocessed_path"], "speakers.json")) as f:
            speaker_map = json.load(f)
        speakers = np.array([speaker_map[args.speaker_id]]) if model_config["multi_speaker"] else np.array([0]) # single speaker is allocated 0
        '''
        spker_embed = np.load(os.path.join(
            preprocess_config["path"]["preprocessed_path"],
            "spker_embed",
            "{}-spker_embed.npy".format(args.speaker_id),
        )) if load_spker_embed else None

        if preprocess_config["preprocessing"]["text"]["language"] == "en":
            texts = np.array([preprocess_english(args.text, preprocess_config)])
        elif preprocess_config["preprocessing"]["text"]["language"] == "zh":
            texts = np.array([preprocess_mandarin(args.text, preprocess_config)])
        #text_lens = np.array([len(texts[0])])
        #file_lists = os.listdir(args.input_path)
        j = 0
        os.makedirs(args.output_path, exist_ok=True)
        language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
        with open (args.input_path,'r') as f1:
          #f1.readlines()
          for line in f1.readlines():
             code_file = line.split('|')[0]+'.npy'


             if len(line.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/test_data/unseen_audio_spkemb/',line.split('|')[-1].rstrip()+'.npy')
             else:
                 speaker_file =  os.path.join('Dataset/preprocessed_data/Group1v2/SpeakerEmb/',code_file)
             print (speaker_file)
             speakers = np.load(speaker_file)
             #int(line.split('|')[3])
             print (speakers)
             print(line.split('|')[2])
             language_id = language_dict[line.split('|')[2]]
             print (code_file)
             if args.code_path == None:
               codes = np.load(os.path.join('Dataset/preprocessed_data/Group1v2/CodeIndex/',code_file))
             else:
               print (os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
               codes = np.load(os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
             print (codes.shape)
             book1 = np.load('Dataset/Group1v2/code_book1.npy')
             book2 = np.load('Dataset/Group1v2/code_book2.npy')
             code_embed = np.empty((codes.shape[0],768))
             #code_embed_2 = numpy.empty((code.shape(0),768))
             i = 0
             print (codes)
             for code in codes:
                 code_embed[i,0:384] = book1[code[0]]
                 code_embed[i,384:768] = book2[code[1]]
                 i = i+1
             #texts = np.array([np.load(os.path.join(args.input_path,code_file))])


             texts = np.array([code_embed])
             #texts = texts
             print ('----texts------',texts)
             print ('----texts------',texts.shape)
             text_lens = np.array([texts.shape[1]])
             print ('-----text_lens-----',text_lens)
             ids = 'pred_mel_'+str(j)
             speakers = np.array([speakers])
             language_id = np.array([language_id])

             batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
             j = j +1

             control_values = args.pitch_control, args.energy_control, args.duration_control

             synthesize(device, model, args, configs, vocoder, batchs, control_values, args.output_path)




    if args.mode == "quick_test":
        ids = raw_texts = [args.text[:100]]
        load_spker_embed = model_config["multi_speaker"] \
            and preprocess_config["preprocessing"]["speaker_embedder"] != 'none'
        '''
        with open(os.path.join(preprocess_config["path"]["preprocessed_path"], "speakers.json")) as f:
            speaker_map = json.load(f)
        speakers = np.array([speaker_map[args.speaker_id]]) if model_config["multi_speaker"] else np.array([0]) # single speaker is allocated 0
        '''
        spker_embed = np.load(os.path.join(
            preprocess_config["path"]["preprocessed_path"],
            "spker_embed",
            "{}-spker_embed.npy".format(args.speaker_id),
        )) if load_spker_embed else None

        #text_lens = np.array([len(texts[0])])
        #file_lists = os.listdir(args.input_path)
        j = 0
        os.makedirs(args.output_path, exist_ok=True)
        language_dict={'English':0,'French':1,'German':2,'Portuguese':3,'Spanish':4,'Swedish':5}
        with open (args.input_path,'r') as f1:
          #f1.readlines()
          for line in f1.readlines():
             code_file = line.split('|')[0]+'.npy'

             if len(line.split('|')) == 6:
                 speaker_file =  os.path.join('Dataset/MM6/test_spk_emb/',line.split('|')[-1].rstrip()+'.npy')
             else:
                 speaker_file =  os.path.join('Dataset/MM6/test_spk_emb/',code_file)
             speakers = np.load(speaker_file)
             #int(line.split('|')[3])
             print (speakers)
             print(line.split('|')[2])
             language_id = language_dict[line.split('|')[2]]
             print (code_file)
             if args.code_path == None:
               codes = np.load(os.path.join('Dataset/preprocessed_data/Group1v2/CodeIndex/',code_file))
             else:
               print (os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
               codes = np.load(os.path.join(args.code_path,'pred_mel_'+str(j)+'.npy'))
             print (codes.shape)
             book1 = np.load('Dataset/MM6/code_book1.npy')
             book2 = np.load('Dataset/MM6/code_book2.npy')
             code_embed = np.empty((codes.shape[0],768))
             #code_embed_2 = numpy.empty((code.shape(0),768))
             i = 0
             print (codes)
             for code in codes:
                 code_embed[i,0:384] = book1[code[0]]
                 code_embed[i,384:768] = book2[code[1]]
                 i = i+1
             #texts = np.array([np.load(os.path.join(args.input_path,code_file))])


             texts = np.array([code_embed])
             #texts = texts
             print ('----texts------',texts)
             print ('----texts------',texts.shape)
             text_lens = np.array([texts.shape[1]])
             print ('-----text_lens-----',text_lens)
             ids = 'pred_mel_'+str(j)
             speakers = np.array([speakers])
             language_id = np.array([language_id])

             batchs = [(ids, raw_texts, speakers, texts, text_lens, max(text_lens), language_id)]
             j = j +1

             control_values = args.pitch_control, args.energy_control, args.duration_control

             synthesize(device, model, args, configs, batchs, control_values, args.output_path)   
