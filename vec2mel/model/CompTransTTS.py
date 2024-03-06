import os
import json

import torch
import torch.nn as nn
import torch.nn.functional as F

from .modules import PostNet, VarianceAdaptor, Generator
from utils.tools import get_mask_from_lengths
from .transformers.blocks import (
    Embedding,
    SinusoidalPositionalEmbedding,
    LayerNorm,
    LinearNorm,
    ConvNorm,
    ConvBlock,
    ConvBlock2D,
)




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

        #self.encoder = TextEncoder(model_config)
        self.codeemb_proj = nn.Linear(768,256)
        #self.variance_adaptor = VarianceAdaptor(preprocess_config, model_config, train_config, self.encoder.d_model)
        self.decoder = Decoder(model_config)
        UP_DOWN_dict = { "upsample_rates": [4,2],"upsample_kernel_sizes": [12,8],"upsample_initial_channel": 256,"resblock_kernel_sizes": [3,7,11],
                "resblock_dilation_sizes": [[1,3,5], [1,3,5], [1,3,5]],}
        self.up_down = Generator(UP_DOWN_dict)
        self.mel_linear = nn.Linear(
            self.decoder.d_model,
            preprocess_config["preprocessing"]["mel"]["n_mel_channels"],
        )

        self.output_norm = nn.LayerNorm(256)
        self.spk_norm = nn.LayerNorm(256)
        self.lang_norm = nn.LayerNorm(256)
        self.spk_proj = nn.Linear(192,256)
        self.lang_proj = nn.Linear(64,256)
        #self.spk_linear = LinearNorm(192, 192)
        '''
        self.spk_linear = nn.Linear(
        192,
        192,)
        '''
        self.postnet = PostNet()
         
        self.language_emb = nn.Embedding(7, 64)
        #self.speaker_emb = nn.Embedding(len(self.speaker_dict), 128)
        
        '''
        self.speaker_emb = None
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
        p_control=1.0,
        e_control=1.0,
        d_control=1.0,
        step=None,
    ):
        src_masks = get_mask_from_lengths(src_lens, max_src_len)
        mel_masks = (
            get_mask_from_lengths(mel_lens, max_mel_len)
            if mel_lens is not None
            else None
        )

        text_embeds = self.codeemb_proj(texts)

        '''
        speaker_embeds = None
        if self.speaker_emb is not None:
            if self.embedder_type == "none":
                speaker_embeds = self.speaker_emb(speakers) # [B, H]
            else:
                assert spker_embeds is not None, "Speaker embedding should not be None"
                speaker_embeds = self.speaker_emb(spker_embeds) # [B, H]
        '''
        output = self.up_down(text_embeds.permute(0,2,1))
        a = output.shape[2]
          
        if mel_lens is None:
             mel_masks=get_mask_from_lengths(torch.tensor(a).unsqueeze(0).cuda(), None)

        '''
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
            texts,
            text_embeds,
            src_lens,
            src_masks,
            mels,
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
        '''
        language_emb = self.language_emb(language_id)
        speakers.requires_grad = False
        speaker_emb = speakers.cuda()
        
        
        outputs = output.permute(0,2,1)
        language_embs = self.lang_proj(language_emb.unsqueeze(1).repeat([1, outputs.shape[1], 1]))
        speaker_embs = self.spk_proj(speaker_emb.repeat([1, outputs.shape[1], 1]))
        out_put_cat = self.output_norm(outputs) + self.lang_norm(language_embs) + self.spk_norm(speaker_embs)
        output, mel_masks = self.decoder(out_put_cat, mel_masks)
        output = self.mel_linear(output)

        postnet_output = self.postnet(output) + output

        p_targets=p_predictions=e_targets=e_predictions=log_d_predictions=d_rounded=attn_outs=prosody_info=p_targets=e_targets=None
        return (
            output,
            postnet_output,
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
