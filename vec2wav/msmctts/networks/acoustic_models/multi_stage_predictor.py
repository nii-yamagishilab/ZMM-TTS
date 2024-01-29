import torch
import torch.nn as nn
import torch.nn.functional as F

from msmctts.utils.utils import get_mask_from_lengths
from .transformer import FFTBlocks, LengthRegulator


class MultiStagePredictor(nn.Module):

    def __init__(self, n_symbols, n_model_size, n_pred_size, n_pred_scale,
                 encoder_config, adaptor_config, decoder_config):
        super(MultiStagePredictor, self).__init__()
        self.n_pred_scale = n_pred_scale

        self.n_symbols = n_symbols
        if isinstance(n_symbols, (tuple, list)):
            self.word_emb = nn.ModuleList([
                nn.Embedding(n_symbol, n_model_size, padding_idx=0)
                for n_symbol in n_symbols
            ])
        else:
            self.word_emb = nn.Embedding(
                n_symbols, n_model_size, padding_idx=0
            )
        self.encoder = FFTBlocks(**encoder_config)

        self.upsampler = LengthRegulator(**adaptor_config)
        
        self.downsamplers =  nn.ModuleList([
            nn.Conv1d(n_model_size, n_model_size, scale * 2 + 1,
                      padding=scale) for scale in n_pred_scale[:: -1]
        ])
        self.decoders = nn.ModuleList([
            nn.ModuleList([
                nn.Linear(n_model_size * 2 + n_pred_size \
                          if i > 0 else n_model_size, n_model_size),
                FFTBlocks(**decoder_config),
                nn.Linear(n_model_size, n_pred_size),
            ]) for i in range(len(n_pred_scale))
        ])

    def forward(self, text, text_length, dur=None, feat=None, feat_length=None):
        
        output, duration = self.encode(text, text_length, dur)

        # Calculate lengths
        if feat_length is None:
            total_length = duration.sum(-1).long()
            feat_length = []
            for scale in self.n_pred_scale[:: -1]:
                total_length = torch.ceil(total_length / scale).long()
                feat_length.append(total_length)
            feat_length = feat_length[:: -1]

        output = self.decode(output, feat, feat_length)

        return {
            'feat': output,
            'feat_length': feat_length,
            'text_length': text_length,
            'duration': duration,
        }

    def encode(self, text, text_length, dur=None):
        # Phoneme Embedding
        if isinstance(self.n_symbols, (tuple, list)):
            output = sum([self.word_emb[i](text[..., i].long())
                for i in range(len(self.word_emb))])
        else:
            output = self.word_emb(text.long())

        # Phoneme Side FFT Blocks
        pos = torch.Tensor([i + 1 for i in range(text_length.max())]).view(
            1, -1).repeat((text.shape[0], 1)).long().to(text.device)
        pos.masked_fill_(get_mask_from_lengths(text_length), 0)
        output, text_mask = self.encoder(output, pos)

        # Length Regulator
        output, _, duration = self.upsampler(
            output, text_mask, target=dur, alpha=1.0)

        return output, duration

    def decode(self, text_embedding, feat=None, feat_lengths=None):
        downsampled_text = []
        for model, scale, feat_length in zip(
                self.downsamplers, self.n_pred_scale[:: -1], feat_lengths[:: -1]):
            text_embedding = model(text_embedding.transpose(1, 2))
            text_embedding = F.avg_pool1d(text_embedding,
                kernel_size=scale, stride=scale,
                ceil_mode=True).transpose(1, 2)
            downsampled_text.append(text_embedding)
        downsampled_text = downsampled_text[:: -1]

        preditions = []
        for i in range(len(self.decoders)):
            decoder = self.decoders[i]
            text_embedding = downsampled_text[i]
            feat_length = feat_lengths[i]

            pos = torch.Tensor([i + 1 for i in range(feat_length.max())]).view(
                1, -1).repeat((text_embedding.shape[0], 1)).long().to(
                text_embedding.device)
            pos.masked_fill_(get_mask_from_lengths(feat_length), 0)

            if i > 0:
                scale = self.n_pred_scale[i - 1]
                pre_input = feat[i - 1] if feat is not None else preditions[-1]
                pre_input = torch.cat((output, pre_input), dim=2)
                pre_input = torch.repeat_interleave(
                    pre_input, scale, dim=1)[:, : text_embedding.shape[1]]
                output = torch.cat((text_embedding, pre_input), dim=2)
            else:
                output = text_embedding

            output = decoder[0](output)
            output, output_mask = decoder[1](output, pos)
            prediction = decoder[2](output)
            if not self.training and hasattr(self, 'quantizers'):
                func = self.quantizers[i].quantize \
                    if hasattr(self.quantizers[i], 'quantize') \
                    else self.quantizers[i]
                prediction = func(prediction)[0]
            preditions.append(prediction)
        return preditions
