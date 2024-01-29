import numpy as np
import torch
import torch.nn.functional as F

from msmctts.utils.utils import to_model
from . import load_task, load_model
from .base_task import BaseTask


class TTS(BaseTask):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        feature_config = self.config.dataset
        self.samplerate = feature_config.samplerate
        self.fs = {feature_config.feature[i]: feature_config.frameshift[i]
                   for i in range(len(feature_config.feature))}

    def train_step(self, input_dict):
        return self.acoustic_model(**input_dict)

    def infer_step(self, input_dict):
        # Generate acoustic features
        if hasattr(self, 'autoencoder') and hasattr(self.autoencoder, 'quantizers'):
            self.acoustic_model.quantizers = self.autoencoder.quantizers
        
        output_dict = self.acoustic_model(**input_dict)

        if hasattr(self, 'autoencoder'):
            if 'pred_embeddings' in output_dict:
                embeddings = output_dict['pred_embeddings']     
                length = output_dict['feat_length']
            else:
                scales = self.autoencoder.downsample_scales
                feat = output_dict.pop('mel')
                preds = list(torch.chunk(feat, len(scales), dim=-1))
                cum_scale, lengths = 1, []
                for i, scale in enumerate(scales):
                    cum_scale *= scale
                    if cum_scale > 1:
                        preds[i] = torch.nn.functional.avg_pool1d(
                            preds[i].transpose(1, 2), cum_scale,
                            stride=cum_scale, ceil_mode=True).transpose(1, 2)
                    lengths.append(torch.ceil(output_dict['mel_length'] / cum_scale).int())
                embeddings, length = preds[:: -1], lengths[:: -1]
            pred = self.autoencoder.synthesis(embeddings, length)

            # Output Mel Spectrogram or Waveform
            if len(pred.shape) == 3 and pred.shape[-1] > 1:
                output_dict['mel'] = pred
            else:
                output_dict['wav'] = pred

        if 'mel' in output_dict:
            pred_mel = output_dict['mel']
            if isinstance(pred_mel, (list, tuple)):
                pred_mel = pred_mel[-1]
            output_dict['mel'] = pred_mel
            
            # Vocoding from acoustic features
            if 'vocoder' in self.config.task:
                if not hasattr(self, 'vocoder'):
                    self.build_vocoder()
                pred_wav = self.vocoder({'mel': pred_mel})['wav']
                output_dict['wav'] = pred_wav
            
            output_dict['mel'] = [x[: l] for x, l in zip(
                output_dict['mel'], output_dict['mel_length'])]

        if 'wav' in output_dict:
            output_dict['wav'] = [x[: l * self.fs['mel']]
                for x, l in zip(output_dict['wav'], output_dict['mel_length'])]

        if 'alignment' in output_dict:
            output_dict['alignment'] = [x[: ol, : il] for x, il, ol in zip(
                output_dict['alignment'], input_dict['text_length'], output_dict['mel_length'])]
        
        return output_dict


class MSMCTTS(TTS):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        feature_config = self.config.dataset
        self.samplerate = feature_config.samplerate
        self.fs = {feature_config.feature[i]: feature_config.frameshift[i]
                   for i in range(len(feature_config.feature))}
        self.training_mode = self.config.task._mode
        self.load_modules = False

    def train_step(self, input_dict, mode=None):
        if mode is None:
            mode = self.training_mode
        if mode == "train_autoencoder":
            return self.analysis_synthesis(input_dict)
        elif mode == "train_predictor":
            return self.predict(input_dict)

    def infer_step(self, input_dict, mode=None):        
        if mode is None:
            mode = self.training_mode

        if mode == "train_autoencoder":
            #print ('---enter--here')
            return self.analysis_synthesis(input_dict)
            #return self.analysis_code(input_dict)

        elif mode == "train_predictor":
            if not self.load_modules:
                self.pre_infer()
            return self.predict(input_dict)

    def predict(self, input_dict):
        # Analysis (optional)
        if 'mel' in input_dict:
            mel = input_dict.pop('mel')
            mel_length = input_dict.pop('mel_length')

        # Prediction
        output_dict = self.predictor(**input_dict)
        feats, lengths = output_dict['feat'], output_dict['feat_length']

        # Synthesis
        wavs = self.autoencoder.synthesis(feats, lengths)[..., 0]

        # Save features
        wav_lengths = (lengths[-1] * wavs.shape[1] / feats[-1].shape[1]).int()
        output_dict['wav'] = [x[: l] for x, l in zip(wavs, wav_lengths)]
        output_dict['embedding'] = feats[-1]
        
        return output_dict

    def analysis_synthesis(self, input_dict):
        # Generate acoustic features
        input_dict['language'] = input_dict['language'].long()
        input_dict['speaker'] = input_dict['speaker'].float()
        output_dict = self.autoencoder(**input_dict)
        #print ('--last output---',output_dict)
        #print ('indices',output_dict['encoder_indices'])
        #print ('indices org',output_dict['encoder_indices'][0])
        #print (output_dict)
        output_dict = {'wav': output_dict['decoder_outputs'].squeeze(-1)}
        #output_dict = {'npy': output_dict['encoder_indices'][0]}
        return output_dict

    def pre_infer(self):
        self.load_modules = True

        # Build Analyzer
        if hasattr(self.config.task, 'autoencoder') and '_checkpoint' in self.config.task.autoencoder:
            checkpoint = self.config.task.autoencoder._checkpoint
            config = self.config.task.autoencoder._config \
                     if hasattr(self.config.task.autoencoder, '_config') \
                     else None
            model = load_model('autoencoder', checkpoint, config)
            model = model.cuda() if torch.cuda.is_available() else model
            model.eval()
            self.autoencoder = model

        # Feed quantizers to acoustic model
        if hasattr(self, 'predictor') and hasattr(self, 'autoencoder'):
            self.predictor.autoencoder = self.autoencoder
            self.predictor.quantizers = self.autoencoder.quantizer.quantizer


    def analysis_code(self, input_dict):
        # Generate acoustic features

        #print (input_dict)
        input_dict['language'] = input_dict['language'].long()
        input_dict['speaker'] = input_dict['speaker'].float()
        output_dict = self.autoencoder.test_code(**input_dict)
        #print ('--last output---',output_dict)
        #print ('indices',output_dict['encoder_indices'])
        #print ('indices org',output_dict['encoder_indices'][0])
        #print (output_dict)
        #print ('shape0',output_dict['encoder_indices'][0].shape)
        #print ('shape1',output_dict['encoder_indices'][1].shape)
        output_dict = {'npy': output_dict['encoder_indices'][1]}
        #output_dict = {'wav': output_dict['decoder_outputs'].squeeze(-1)}
        #output_dict = {'npy': output_dict['encoder_indices'][0]}
        return output_dict
