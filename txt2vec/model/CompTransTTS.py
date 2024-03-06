import os
import json

import torch
import torch.nn as nn
import torch.nn.functional as F

from .modules import PostNet, VarianceAdaptor
from utils.tools import get_mask_from_lengths, get_mask_from_lengths_r
import numpy as np
from transformers import AutoTokenizer, AutoModel

class Cls(nn.Module):
    def __init__(self, input_dim, spk_num):
        super(Cls, self).__init__()
        #self.input_dim = styler_dim
        #self.layers_num = layers
        self.linear_layer1 = nn.Linear(input_dim, input_dim)
        self.linear_layer2 = nn.Linear(input_dim, spk_num)
 
    def forward(self, input_embed):
        input_embed1= self.linear_layer1(input_embed)
        input_embed2= self.linear_layer2(input_embed1)
        return  F.log_softmax(input_embed2)


class CompTransTTS(nn.Module):
    """ CompTransTTS """

    def __init__(self, preprocess_config, model_config, train_config):
        super(CompTransTTS, self).__init__()
        self.model_config = model_config

        if model_config["block_type"] == "transformer_fs2":
            from .transformers.transformer_fs2 import TextEncoder, Decoder
        elif model_config["block_type"] == "transformer":
            from .transformers.transformer import TextEncoder, Decoder
        elif model_config["block_type"] == "lstransformer":
            from .transformers.lstransformer import TextEncoder, Decoder
        elif model_config["block_type"] == "fastformer":
            from .transformers.fastformer import TextEncoder, Decoder
        elif model_config["block_type"] == "conformer":
            from .transformers.conformer import TextEncoder, Decoder
        elif model_config["block_type"] == "reformer":
            from .transformers.reformer import TextEncoder, Decoder
        else:
            raise NotImplementedError
        
        
        if not model_config['XphoneBERT']['useEncoder']:
           self.encoder = TextEncoder(model_config)
           #Just for using our paper's model
           #self.xpemb_attt_proj = nn.Linear(768,256)
        else:
           self.xpemb_attt_proj = nn.Linear(768,256)
           self.bert = AutoModel.from_pretrained("vinai/xphonebert-base").cuda()
        self.variance_adaptor = VarianceAdaptor(preprocess_config, model_config, train_config, 256)
        self.decoder = Decoder(model_config)
        self.postnet = PostNet()
       
        if model_config['use_lang']:
           self.lan_norm1 = nn.LayerNorm(64)
           self.lan_norm2 = nn.LayerNorm(256)
           self.language_emb = nn.Embedding(7, 64)
           self.language_emb_proj = nn.Linear(64,256)

        self.text_norm = nn.LayerNorm(256)
        self.decoder_norm = nn.LayerNorm(256)
        self.spk_norm = nn.LayerNorm(256)

        self.glumb_linear = nn.Linear(
            self.decoder.d_model,
            640
            )
        self.mel_attt_proj = nn.Linear(
            preprocess_config["preprocessing"]["mel"]["n_mel_channels"],
            preprocess_config["preprocessing"]["mel"]["n_mel_proj"],
        )

        self.spk_norm_att =  nn.LayerNorm(192)
        self.speaker_emb_proj = nn.Linear(192,256)

        self.speaker_emb = None
        '''
        if model_config["multi_speaker"]:
            self.embedder_type = preprocess_config["preprocessing"]["speaker_embedder"]
            if self.embedder_type == "none":
                with open(
                    os.path.join(
                        preprocess_config["path"]["preprocessed_path"], "speakers.json"
                    ),
                    "r",
                ) as f:
                    n_speaker = len(json.load(f))
                self.speaker_emb = nn.Embedding(
                    n_speaker,
                    self.encoder.d_model,
                )
            else:
                self.speaker_emb = nn.Linear(
                    model_config["external_speaker_dim"],
                    self.encoder.d_model,
                )
        '''

    def forward(
        self,
        speakers,
        texts,
        src_lens,
        max_src_len,
        mels=None,
        mel_lens=None,
        max_mel_len=None,
        p_targets=None,
        e_targets=None,
        d_targets=None,
        attn_priors=None,
        language_id=None,
        DRs=None,
        p_control=1.0,
        e_control=1.0,
        d_control=1.0,
        step=None,
    ):
        src_masks = get_mask_from_lengths(src_lens, max_src_len)
        
        
        if self.model_config['XphoneBERT']['useEncoder']:
            src_masks_r = get_mask_from_lengths_r(src_lens, max_src_len)
            x = self.bert(input_ids=texts, attention_mask=src_masks_r) 
            text_embeds1 = self.xpemb_attt_proj(x[0])
            texts1 = text_embeds1
            for child in self.bert.children():
                for param in child.parameters():
                    param.requires_grad = self.model_config['XphoneBERT']['needUpdate']
        else:
            #print ('22222',texts.shape)
            texts1, text_embeds1 = self.encoder(texts, src_masks)

        mel_masks = (
            get_mask_from_lengths(mel_lens, max_mel_len)
            if mel_lens is not None
            else None
        )
        if mels is not None:
            mels_1 = F.one_hot(mels[:,:,0],320)
            mels_2 = F.one_hot(mels[:,:,1],320)
            mels = torch.cat([mels_1,mels_2],-1).float()
            mels_proj_atten =  self.mel_attt_proj (mels)
        else:
            mels_proj_atten = mels
         
        speaker_embeds = None 
        speakers.requires_grad=False
        speaker_emb = speakers
        speaker_myembs1 = speaker_emb.repeat([1, text_embeds1.shape[1], 1]) 
        
        if self.model_config['use_lang']:
           language_emb = self.language_emb(language_id)
           language_embs = language_emb.unsqueeze(1).repeat([1, text_embeds1.shape[1], 1])
           text_embeds = torch.cat([self.text_norm(text_embeds1), self.lan_norm1(language_embs),self.spk_norm_att(speaker_myembs1)], dim=-1)
        else:
           text_embeds = torch.cat([self.text_norm(text_embeds1),self.spk_norm_att(speaker_myembs1)], dim=-1)
        if self.speaker_emb is not None:
            if self.embedder_type == "none":
                speaker_embeds = self.speaker_emb(speakers) # [B, H]
            else:
                assert spker_embeds is not None, "Speaker embedding should not be None"
                speaker_embeds = self.speaker_emb(spker_embeds) # [B, H]

        (
            output,
            p_targets,
            p_predictions,
            e_targets,
            e_predictions,
            log_d_predictions,
            d_rounded,
            mel_lens,
            mel_masks,
            attn_outs,
            prosody_info,
        ) = self.variance_adaptor(
            speaker_embeds,
            texts1,
            text_embeds,
            src_lens,
            src_masks,
            mels_proj_atten,
            mel_lens,
            mel_masks,
            max_mel_len,
            p_targets,
            e_targets,
            d_targets,
            attn_priors,
            p_control,
            e_control,
            d_control,
            step,
        )

        if self.model_config['use_lang']:
            output = self.decoder_norm(output) + self.lan_norm2(self.language_emb_proj(language_emb.unsqueeze(1).repeat([1, output.shape[1], 1]))) +self.spk_norm(self.speaker_emb_proj(speaker_emb).repeat([1, output.shape[1], 1]))
        else:
            output = self.decoder_norm(output) + self.spk_norm(self.speaker_emb_proj(speaker_emb).repeat([1, output.shape[1], 1]))
        output, mel_masks = self.decoder(output, mel_masks)
        
        #print ('---orgoutput----', output.shape)

        output_org = self.glumb_linear(output)
        #print ('------',output.shape)
        #print (output[0,0,:320])
        output = output_org.view(output_org.shape[0],output_org.shape[1],2,320)
        '''
        output = output.view(output.shape[0],output.shape[1],640)
        output = F.gumbel_softmax(output, tau=2, hard=True) 
        print ('------',output[0,0,0])
        print ('111111',output[0,0,1])
        output = output.unsqueeze(-1).repeat([1, 1, 1, 1, 384])
        #print (output[0,0,0,145])
        print (self.code_book.shape)
        output1= output * self.code_book1
        print ('output1--shape',output1.shape)
        print (output1[0,0,0,146])
        print ('---second-----')
        print (self.code_book[1,102])
        print (output1[0,0,1,102])
        output2 = output1.sum(-2)
        #print ('---sum----',output2)
        output3 = output2.view(output2.shape[0],output2.shape[1],768)
        #print ('---final----',output3)
        print ('------',output3.shape)
        print ('------',output3[0,0])
        '''

        #print ('---output----',output.shape)
        '''
        output_1 = self.codecls_1(output[:,:,0:384])
        output_2 = self.codecls_2(output[:,:,384:])
        output_fin1 = torch.cat([output_1,output_2],-1)
        postnet_output = self.postnet(output) + output
        postnet_output_1 = self.codecls1(postnet_output[:,:,0:384])
        postnet_output_2 = self.codecls2(postnet_output[:,:,384:])
        #print (postnet_output_2.shape)
        postnet_output_fin1 = torch.cat([postnet_output_1,postnet_output_2],-1)
        #print (postnet_output_fin1.shape)
        '''
        
        
        
        
        return (
            output_org,
            output,
            p_predictions,
            e_predictions,
            log_d_predictions,
            d_rounded,
            src_masks,
            mel_masks,
            src_lens,
            mel_lens,
            attn_outs,
            prosody_info,
            p_targets,
            e_targets,
        )
