from turtle import update
from torch import nn
from torch.nn import functional as F

import numpy as np
import torch

from msmctts.utils.utils import get_mask_from_lengths
from msmctts.networks.acoustic_models.transformer import FFTBlocks
from msmctts.networks.hifigan import HifiGANGenerator
from .modules import *


class MultiStageEncoder(nn.Module):
    def __init__(self, in_channels,
                downsample_scales=[1],
                max_seq_len=2400,
                n_layers=4,
                n_head=2,
                d_k=64,
                d_v=64,
                d_inner=1024,
                fft_conv1d_kernel=3,
                fft_conv1d_padding=1,
                dropout=0.2,
                attn_dropout=0.1,
                fused_layernorm=False):
        super(MultiStageEncoder, self).__init__()
        self.downsample_scales = downsample_scales
        self.encoders = nn.ModuleList([
            FFTBlocks(max_seq_len=max_seq_len,
                      n_layers=n_layers,
                      n_head=n_head,
                      d_k=d_k,
                      d_v=d_v,
                      d_model=in_channels,
                      d_inner=d_inner,
                      fft_conv1d_kernel=fft_conv1d_kernel,
                      fft_conv1d_padding=fft_conv1d_padding,
                      dropout=dropout,
                      attn_dropout=attn_dropout,
                      fused_layernorm=fused_layernorm,
                      name='encoder_{}'.format(i))
        for i in range(len(downsample_scales))])

    def forward(self, input, input_length):
        outputs = []
        
        feat, feat_length = input, input_length
        for encoder, scale in zip(self.encoders, self.downsample_scales):
            if scale > 1:
                feat = F.avg_pool1d(feat.transpose(1, 2),
                                     kernel_size=scale, stride=scale,
                                     ceil_mode=True).transpose(1, 2)
                feat_length = torch.ceil(feat_length / scale).int()
            pos = torch.Tensor([i + 1 for i in range(feat_length.max())]).view(
                1, -1).repeat((feat.shape[0], 1)).long().to(feat.device)
            pos.masked_fill_(get_mask_from_lengths(feat_length), 0)
            feat, _ = encoder(feat, pos)
            outputs.append((feat, feat_length))

        return outputs


class PriorPredictor(nn.Module):
    def __init__(self,
                in_channels,
                out_channels,
                kernel_size=5,
                dilation_rate=1,
                n_layers=4):
        super().__init__()
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size
        self.dilation_rate = dilation_rate
        self.n_layers = n_layers

        self.enc = ResStack(in_channels, kernel_size,
                            dilation_rate, n_layers)
        self.proj = nn.Conv1d(in_channels, out_channels, 1)

    def forward(self, x, x_lengths):
        x = x.transpose(1, 2)
        x_mask = ~get_mask_from_lengths(x_lengths).unsqueeze(1)
        x = self.enc(x, x_mask)
        o = self.proj(x) * x_mask
        return x.transpose(1, 2), o.transpose(1, 2)


class MultiStageQuantizer(nn.Module):
    def __init__(self, n_model_size, upsample_scales,
                 embedding_sizes=512, embedding_dims=256, n_heads=4,
                 prior_config={}, norm=False, upsampling='repeat',
                 dropout=0.1, update_codebook=True):
        super(MultiStageQuantizer, self).__init__()
        self.upsample_scales = upsample_scales
        self.upsampling = upsampling
        self.dropout = dropout
        self.update_codebook = update_codebook

        self.quantizer = nn.ModuleList([])
        self.predictor = nn.ModuleList([])
        self.preprocessor = nn.ModuleList([])
        self.postprocessor = nn.ModuleList([])
        if upsampling != 'repeat':
            self.transposed_conv = nn.ModuleList([])

        for i in range(len(upsample_scales)):
            # Add predictor (useless at highest stage)
            predictor = PriorPredictor(n_model_size, embedding_dims, **prior_config)
            self.predictor.append(predictor)
            
            # Add pre-processor
            preprocessor = [
                nn.Conv1d(n_model_size * (1 if i == 0 else 2), embedding_dims, 1),
                nn.Tanh(),
                nn.Conv1d(embedding_dims, embedding_dims, 1),
            ]
            if norm:
                preprocessor.append(nn.BatchNorm1d(embedding_dims, eps=1e-05, affine=False))
            preprocessor = nn.Sequential(*preprocessor)
            self.preprocessor.append(preprocessor)
            
            # Add quantizer
            quantizer = Quantize(embedding_dims, embedding_sizes) if n_heads == 1 else \
                MultiHeadQuantize(embedding_dims, embedding_sizes, n_heads)
            self.quantizer.append(quantizer)
            
            # Add Post-processor
            postprocessor = nn.Sequential(
                nn.Linear((embedding_dims * (1 if i == 0 else 2)), embedding_dims),
                nn.Tanh(),
                nn.Linear(embedding_dims, n_model_size)
            )
            self.postprocessor.append(postprocessor)

            # (Optional) Add transposed Upsampler
            if self.upsampling != 'repeat':
                u = upsample_scales[i]
                k = u * 2 if u % 2 == 0 else u * 2 + 1
                p = (k - u) // 2
                self.transposed_conv.append(nn.ConvTranspose1d(
                    n_model_size, n_model_size, k, u, padding=p))


    def forward(self, encoder_states, from_encoder=True):
        # Init
        quant_states, pred_states = [], []
        residual_output = None
        encoder_states1 = encoder_states
        # Reverse encoder outputs
        Test_code = False
        if from_encoder:
            if Test_code:
                encoder_states = zip(encoder_states, encoder_states)
            else:
                encoder_states = encoder_states[:: -1]
     
        # if not self.training:
        #     encoder_states = [list(x) for x in encoder_states]
        #     encoder_states[1][0] = None
        #print ('---residual_output---',residual_output)
        # Quantize in stages
        for i, (embedding, length) in enumerate(encoder_states):
            # Predict i-th Quantized Sequence
            if residual_output is None:
                pred_quant = None
            else:
                residual_output = residual_output[:, : length.max()]
                pred_hidden, pred_quant = self.predictor[i](residual_output, length)
                residual_output = residual_output + F.dropout(
                    pred_hidden, p=self.dropout, training=self.training)

            # Pre Quantization
            if embedding is None:
                quantizer_input = pred_quant
            elif from_encoder:
                if Test_code:
                    pass
                else:
                    #print ('from_encoder')
                    pre_inputs = torch.cat((embedding, residual_output), dim=-1) \
                    if residual_output is not None else embedding
                    quantizer_input = self.preprocessor[i](
                    pre_inputs.transpose(1, 2)).transpose(1, 2)
            else:
                #print ('embed')
                quantizer_input = embedding


            
            if Test_code: 
                quantizer_input = encoder_states1.long()
                print ('quantizer_input',quantizer_input.shape)
                print ('quantizer_input',quantizer_input)
                quant, diffs, indices = self.quantizer[i](
                                       quantizer_input, length, update=self.update_codebook)
            else:
                #print ('quantizer_input_com',quantizer_input.shape)
                # Quantization
                quant, diffs, indices = self.quantizer[i](
                quantizer_input, length, update=self.update_codebook)

            # Post Quantization
            post_inputs = quant if residual_output is None else \
                torch.cat((residual_output, quant), dim=-1)
            #print ('post_inputs',post_inputs.shape)
            #post_inputs = post_inputs.squeeze(2)
            #print ('post_inputs',post_inputs.shape)
            post_outputs = self.postprocessor[i](post_inputs)
            post_outputs = F.dropout(post_outputs,
                                     p=self.dropout, training=self.training)
            residual_output = post_outputs if residual_output is None else \
                residual_output + post_outputs
            
            # Save states
            quant_states.append((quant, diffs, indices))
            pred_states.append({
                'predictor_outputs': pred_quant,
                'target_outputs': quant,
                'target_indices': indices,
                'target_lengths': length})

            # Prepare for the next stage
            if self.upsampling == 'mapping':
                residual_output = self.transposed_conv[i](
                    residual_output.transpose(1, 2)).transpose(1, 2)
            elif self.upsampling == 'residual':
                transposed_output = self.transposed_conv[i](
                    residual_output.transpose(1, 2)).transpose(1, 2)
                residual_output = torch.repeat_interleave(residual_output,
                    self.upsample_scales[i], dim=1) + F.dropout(
                        transposed_output, p=self.dropout, training=self.training)
            elif self.upsampling == 'repeat':
                residual_output = torch.repeat_interleave(residual_output,
                    self.upsample_scales[i], dim=1)

        # Output
        quant_outputs, quant_diffs, quant_indices = zip(*quant_states)
        output_dict = {
            'residual_output': residual_output,
            'quantizer_outputs': quant_outputs,
            'quantizer_diffs': quant_diffs,
            'quantizer_indices': quant_indices,
            'quantizer_lengths': [x[1] for x in encoder_states]
        }

        # Calculate prediction loss
        predictor_diffs = None
        if self.training:
            predictor_diffs = self.compute_embedding_loss(pred_states,
                methods=['mse'], loss_weights=[1.0])
        output_dict['predictor_diffs'] = predictor_diffs

        return output_dict

    def compute_embedding_loss(self, pred_states,
                               methods=['mse'], loss_weights=[1.0]):
        loss_dict = {'total_loss': 0}
        for i, state in enumerate(pred_states):
            # Skip highest stage
            p = state['predictor_outputs']
            if p is None:
                continue
            
            weights = loss_weights
            if isinstance(loss_weights[0], (list, tuple)):
                weights = loss_weights[i]

            # Calculate loss
            for method, weight in zip(methods, weights):
                if method == 'mse':
                    t = state['target_outputs']
                    loss = F.mse_loss(p, t.detach(), reduction='none').mean(-1)
                elif method == 'softmax':
                    t = state['target_indices']
                    B, T, D = p.shape
                    loss = F.cross_entropy(p.view(-1, D), t.detach().view(-1),
                                           reduction='none').view(B, T)
                elif method in ['triple', 'triple_mean']:
                    loss = self.quantizer[i].compute_triple_loss(
                            p, state['target_indices'])
                elif method == 'triple_sum':
                    loss = self.quantizer[i].compute_triple_loss(
                            p, state['target_indices'], reduction='sum')

                mask = get_mask_from_lengths(state['target_lengths'])
                loss.masked_fill_(mask, 0)
                loss = loss.sum() / sum(state['target_lengths'])

                loss_dict['embed_loss_{}_{}'.format(method, i)] = loss
                loss_dict['total_loss'] += loss * weight

        return loss_dict


class MSMCVQGAN(nn.Module):

    def __init__(self, in_dim, n_model_size,
                 encoder_config=None,
                 quantizer_config=None,
                 frame_decoder_config=None,
                 decoder_config=None,
                 pred_mel=False):
        super(MSMCVQGAN, self).__init__()
        self.in_linear = nn.Linear(in_dim, n_model_size)
        
        self.language_emb = nn.Embedding(7, 64)
        self.language_norm = nn.LayerNorm(64)
        self.spk_norm = nn.LayerNorm(192)
        self.decoder_norm = nn.LayerNorm(256)
        #self.speaker_emb = nn.Embedding(547, 128)
        # Build Encoder
        self.encoder = MultiStageEncoder(n_model_size, **encoder_config)
        
        self.quantizer = MultiStageQuantizer(
            n_model_size, encoder_config['downsample_scales'][:: -1],
            **quantizer_config)
        
        # Decoder
        #print ('--n_model_size-----',n_model_size)
        decoder_config['num_mels'] = n_model_size
        #print ('--decoder--config',decoder_config)
        self.decoder = HifiGANGenerator(**decoder_config)
        self.decoder_config = decoder_config 

        # (Optional) Frame Decoder
        if frame_decoder_config is not None:
            self.frame_decoder = FFTBlocks(
                d_model=n_model_size,
                name='frame_decoder',
                **frame_decoder_config)
        
        # (Optional) Mel predictor
        if pred_mel:
            self.mel_predictor = nn.Linear(n_model_size, in_dim)

    def forward(self, mel, mel_length,language,speaker, warmup=False, window=None):
        output_dict = {}
        #print ('------enter-forward')
        language_emb = self.language_emb(language)
         
        speaker_emb = speaker
        speaker_emb.requires_grad = False
        #print ('123',speaker_emb.shape)
        #print ('456',language_emb.shape)
        #print (speaker_emb.shape)
        #print (speaker_emb)
        #speaker_emb = self.speaker_emb(speaker)
        #print (language_emb.shape)
        #print (speaker_emb.shape)
        # Encode
        #print (mel_length)
        encoder_inputs = self.in_linear(mel)
        #print (encoder_inputs.shape)
        encoder_states = self.encoder(encoder_inputs, mel_length)
        #print ('--encoder_states----',encoder_states)
        # Quantize
        quantizer_states = self.quantizer(encoder_states)
        decoder_inputs = quantizer_states['residual_output']
        #print ('--decoder_inputs----',decoder_inputs)
        #print ('--decoder_inputs----',decoder_inputs.shape)
        encoder_outputs, encoder_lengths = zip(*encoder_states)
        output_dict.update({
            'encoder_outputs': encoder_outputs[:: -1],
            'encoder_lengths': encoder_lengths[:: -1],
            'encoder_indices': quantizer_states['quantizer_indices'],
            'encoder_diffs': quantizer_states['quantizer_diffs'],
            'decoder_diffs': quantizer_states['predictor_diffs'],
        })

        # (Optional) Frame Decoder
        if hasattr(self, 'frame_decoder'):
            pos = torch.Tensor([i + 1 for i in range(mel_length.max())]).view(
                1, -1).repeat((mel.shape[0], 1)).long().to(mel.device)
            pos.masked_fill_(get_mask_from_lengths(mel_length), 0)
            decoder_inputs, _ = self.frame_decoder(decoder_inputs, pos)

        # (Optional) Mel predictor
        if hasattr(self, 'mel_predictor'):
            mel_outputs = self.mel_predictor(decoder_inputs)
            output_dict['mel_outputs'] = mel_outputs

        # Waveform Generation
        #warmup=False
        if not warmup:
            #print ('---not warmup')
            if window is not None:
                #print ('window',window)
                #print ('--len---',len(window))
                assert len(window) == decoder_inputs.shape[0]
                decoder_inputs = torch.stack([decoder_inputs[i, s: e]
                    for i, (s, e) in enumerate(window)], dim=0)
            #print ('decoder_inputs',decoder_inputs.shape)
            if self.decoder_config['use_lang']:
               language_embs = self.language_norm(language_emb.unsqueeze(1).repeat([1, decoder_inputs.shape[1], 1]))
            speaker_embs = self.spk_norm(speaker_emb.repeat([1, decoder_inputs.shape[1], 1]))
            #print (language_embs.shape)
            #print('----',decoder_inputs.shape)
            if self.decoder_config['use_lang']:
               decoder_inputs = torch.cat([self.decoder_norm(decoder_inputs), language_embs,speaker_embs], dim=-1)
            else:
               #print ('1111111111111111111')
               decoder_inputs = torch.cat([self.decoder_norm(decoder_inputs), speaker_embs], dim=-1)
            decoder_outputs = self.decoder(decoder_inputs.transpose(1, 2)).transpose(1, 2)
            output_dict['decoder_outputs'] = decoder_outputs
        #print (output_dict)
        #print (output_dict)
        return output_dict

    def analysis(self, mel, mel_length):
        # Encode
        encoder_inputs = self.in_linear(mel)
        encoder_states = self.encoder(encoder_inputs, mel_length)

        # Quantize
        quantizer_states = self.quantizer(encoder_states)

        if self.training:
            encoder_outputs, encoder_lengths = zip(*encoder_states)
            return {
                'encoder_outputs': encoder_outputs[:: -1],
                'encoder_lengths': encoder_lengths[:: -1],
                'encoder_indices': quantizer_states['quantizer_indices'],
                'encoder_diffs': quantizer_states['quantizer_diffs'],
                'decoder_diffs': quantizer_states['predictor_diffs'],
                'quantizer_states': quantizer_states
            }
        return quantizer_states

    def synthesis(self, quantizer_outputs, quantizer_lengths):
        # Quantize
        quantizer_states = quantizer_outputs
        if not isinstance(quantizer_outputs, dict):
            quantizer_states = self.quantizer(zip(quantizer_outputs, quantizer_lengths),
                                              from_encoder=False)
        decoder_inputs = quantizer_states['residual_output']

        # Frame Decoder
        if hasattr(self, 'frame_decoder'):
            decoder_length = quantizer_lengths[-1]
            pos = torch.Tensor([i + 1 for i in range(decoder_length.max())]).view(
                1, -1).repeat((decoder_inputs.shape[0], 1)).long().to(decoder_inputs.device)
            pos.masked_fill_(get_mask_from_lengths(decoder_length), 0)
            decoder_inputs, _ = self.frame_decoder(decoder_inputs, pos)

        # Decode
        decoder_outputs = self.decoder(decoder_inputs.transpose(1, 2)).transpose(1, 2)

        if self.training:
            output_dict = {'decoder_outputs': decoder_outputs}
            if hasattr(self, 'mel_predictor'):
                mel_outputs = self.mel_predictor(decoder_inputs)
                output_dict['mel_outputs'] = mel_outputs
            return output_dict

        return decoder_outputs


    def compute_embedding_loss(self, quantizer_outputs, quantizer_lengths, quantizer_states,
                               methods=['mse'], loss_weights=[1.0]):
        pred_states = [{
            'predictor_outputs': quantizer_outputs[i],
            'target_outputs': quantizer_states['quantizer_outputs'][i],
            'target_indices': quantizer_states['quantizer_indices'][i],
            'target_lengths': quantizer_lengths[i]
        } for i in range(len(quantizer_outputs))]

        return self.quantizer.compute_embedding_loss(pred_states, methods, loss_weights)

    def test_code(self, mel, mel_length,language,speaker, warmup=False, window=None):
        output_dict = {}
        #print ('------enter-forward')
        # Encode
        #encoder_inputs = self.in_linear(mel)
        #encoder_states = self.encoder(encoder_inputs, mel_length)

        # Quantize
        #code_index = mel
        language_emb = self.language_emb(language)
        speaker_emb = speaker
        encoder_inputs = self.in_linear(mel)
        #print (encoder_inputs.shape)
        encoder_states = self.encoder(encoder_inputs, mel_length)
        quantizer_states = self.quantizer(encoder_states)
       
        #print ('---quantizer_indices---',quantizer_states)
        decoder_inputs = quantizer_states['residual_output']
        #decoder_inputs = quantizer_states['residual_output'].squeeze(2)

        #print ('-----decoder_inputs------',decoder_inputs.shape)

        #encoder_outputs, encoder_lengths = zip(*encoder_states)
        '''
        output_dict.update({
            'encoder_outputs': encoder_outputs[:: -1],
            'encoder_lengths': encoder_lengths[:: -1],
            'encoder_indices': quantizer_states['quantizer_indices'],
            'encoder_diffs': quantizer_states['quantizer_diffs'],
            'decoder_diffs': quantizer_states['predictor_diffs'],
        })
        '''

        # (Optional) Frame Decoder
        if hasattr(self, 'frame_decoder'):
            pos = torch.Tensor([i + 1 for i in range(mel_length.max())]).view(
                1, -1).repeat((mel.shape[0], 1)).long().to(mel.device)
            pos.masked_fill_(get_mask_from_lengths(mel_length), 0)
            decoder_inputs, _ = self.frame_decoder(decoder_inputs, pos)

        # (Optional) Mel predictor
        if hasattr(self, 'mel_predictor'):
            mel_outputs = self.mel_predictor(decoder_inputs)
            output_dict['mel_outputs'] = mel_outputs

        # Waveform Generation
        if not warmup:
            #print ('---not warmup')
            if window is not None:
                #print ('window',window)
                #print ('--len---',len(window))
                assert len(window) == decoder_inputs.shape[0]
                decoder_inputs = torch.stack([decoder_inputs[i, s: e]
                    for i, (s, e) in enumerate(window)], dim=0)
            #print ('decoder_inputs',decoder_inputs.shape)
            language_embs = self.language_norm(language_emb.unsqueeze(1).repeat([1, decoder_inputs.shape[1], 1]))
            speaker_embs = self.spk_norm(speaker_emb.repeat([1, decoder_inputs.shape[1], 1]))
            #print (language_embs.shape)
            #print('----',decoder_inputs.shape)
            decoder_inputs = torch.cat([self.decoder_norm(decoder_inputs), language_embs,speaker_embs], dim=-1) 
            decoder_outputs = self.decoder(decoder_inputs.transpose(1, 2)).transpose(1, 2)
            output_dict['decoder_outputs'] = decoder_outputs
            output_dict['encoder_indices'] = quantizer_states['quantizer_indices']
        #print (output_dict)
        #print (output_dict)
        return output_dict
