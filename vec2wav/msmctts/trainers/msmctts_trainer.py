import random
import torch
import torch.nn as nn
import torch.nn.functional as F

from msmctts.tasks import load_model
from msmctts.utils.utils import get_mask_from_lengths
from .base_trainer import BaseTrainer
from .criterions.stft_loss import MelLoss, MultiResolutionSTFTLoss


class DurationLoss(nn.Module):
    def __init__(self, lambda_dur=1):
        super(DurationLoss, self).__init__()
        self.lambda_dur = lambda_dur
        self.mse_loss = nn.MSELoss(reduction='none')

    def forward(self, outputs, targets):
        loss = {'total_loss': 0}

        # Duration Loss
        dur_target = targets['dur'].float()
        dur_target.requires_grad = False

        dur_length = targets['text_length']
        dur_mask = get_mask_from_lengths(dur_length)

        dur_pred = outputs['duration']
        dur_loss = self.mse_loss(dur_pred, dur_target)
        dur_loss.masked_fill_(dur_mask, 0)
        dur_loss = dur_loss.sum() / sum(dur_length)

        loss['dur_loss'] = dur_loss
        loss['total_loss'] = loss['total_loss'] + self.lambda_dur * dur_loss

        return loss


class QuantizerLoss(nn.Module):
    def __init__(self, lambda_vq=1, lambda_pr=1):
        super(QuantizerLoss, self).__init__()
        self.lambda_vq = lambda_vq
        self.lambda_pr = lambda_pr

    def forward(self, outputs):
        loss = {'vq_loss': 0}

        latent_losses = outputs['encoder_diffs']
        if not isinstance(latent_losses, (tuple, list)):
            latent_losses = [latent_losses]

        for i, latent_loss in enumerate(latent_losses):
            if 'encoder_lengths' in outputs:
                latent_length = outputs['encoder_lengths'][i]
                latent_mask = get_mask_from_lengths(latent_length)
            if not isinstance(latent_loss, (tuple, list)):
                latent_loss = [latent_loss]
            for j, term in enumerate(latent_loss):
                term.masked_fill_(latent_mask.unsqueeze(-1), 0)
                term = term.sum() / sum(latent_length) / term.shape[2]
                loss['latent_loss_{}_{}'.format(i, j)] = term
                loss['vq_loss'] = loss['vq_loss'] + self.lambda_vq * term
            
        if 'decoder_diffs' in outputs:
            latent_losses = outputs['decoder_diffs']
            if isinstance(latent_losses, dict):
                latent_loss = self.lambda_pr * latent_losses.pop('total_loss')
                loss['vq_loss'] = loss['vq_loss'] + latent_loss
                loss.update(latent_losses)

        return loss


class VQGANTrainer(BaseTrainer):
    def __init__(self, config, model, num_gpus=1, rank=0,
                 warmup_steps=0,
                 lambda_frame=1.0,
                 eval_inteval_iters=1000,
                 grad_clip_thresh=1.0,
                 sample_lengths=16000,
                 lambda_vq=1, lambda_pr=1, lambda_fm=2, lambda_stft=45,
                 stft_loss_func='mel_loss',
                 stft_loss_config=None):
        super().__init__(config, model, num_gpus, rank)
        self.lambda_frame = lambda_frame
        self.warmup_steps = warmup_steps
        self.frameshift = self.config.dataset.frameshift[self.config.dataset.feature.index('mel')]
        self.frame_lengths = -1 if sample_lengths == -1 else \
                    sample_lengths // self.frameshift

        self.eval_inteval_iters = eval_inteval_iters
        self.grad_clip_thresh = grad_clip_thresh
        self.vq_criterion = QuantizerLoss(lambda_vq=lambda_vq,
                                          lambda_pr=lambda_pr)
        self.sample_lengths = sample_lengths
        self.lambda_fm = lambda_fm
        self.lambda_stft = lambda_stft
        self.criterion = nn.MSELoss()
        self.l1_loss = nn.L1Loss()

        kwargs = {}
        #print ('---loss----',stft_loss_func)
        if stft_loss_func == 'mel_loss':
            kwargs['sample_rate'] = config.dataset.samplerate
            kwargs['win_size'] = kwargs['sample_rate'] // 20
            kwargs['hop_size'] = kwargs['sample_rate'] // 80
            kwargs['fft_size'] = 2048 if kwargs['win_size'] > 1024 else 1024
            kwargs['num_mels'] = 128
            if stft_loss_config is not None:
                kwargs.update(stft_loss_config)
            #print ('stft_loss_func kwargs',kwargs)
            self.stft_criterion = MelLoss(**kwargs)
            #print ('stft_loss_func',stft_loss_func)
        elif stft_loss_func == 'mr_stft':
            #print ('mr-----stft',)
            kwargs.update(stft_loss_config)
            self.stft_criterion = MultiResolutionSTFTLoss(**kwargs)
        #print ('---kwargs----',kwargs)
    def train_step(self, batch, iteration):
        losses = {}
        #print (batch)
        mel, mel_length = batch['mel'], batch['mel_length']
        wav, wav_length = batch['wav'], batch['wav_length']
        language, speaker = batch['language'].long(), batch['speaker'].float()
        #print (language.shape)
        #print (mel.shape)
        #print (speaker)
        # Analysis
        if iteration < self.warmup_steps:
            #print ('---target---1--enter')
            output = self.model.autoencoder(mel, mel_length,language,speaker, warmup=True)
        else:
            #print ('---target---1')
            frame_windows, sample_windows = self.random_select(mel_length)
            target = torch.stack([wav[i, s: e]
                for i, (s, e) in enumerate(sample_windows)], dim=0)
            #print ('---target---1',target.shape)
            #print ('---target---1--shape',mel_length,sample_windows,frame_windows)
            output = self.model.autoencoder(mel, mel_length,language,speaker,
                warmup=False, window=frame_windows)

        # VQ loss
        vq_loss = self.vq_criterion(output)
        losses.update(vq_loss)
        g_loss = vq_loss['vq_loss']
        
        # (Optional) Mel predictor
        if 'mel_outputs' in output:
            pred_mel = output['mel_outputs']
            mel_loss = F.mse_loss(mel, pred_mel, reduction='none')
            mel_mask = get_mask_from_lengths(mel_length)
            mel_loss.masked_fill_(mel_mask.unsqueeze(-1), 0)
            mel_loss = mel_loss.sum() / sum(mel_length) / mel_loss.shape[2]
            losses['frame_loss'] = mel_loss.item()
            g_loss = g_loss + self.lambda_frame * mel_loss

        # Waveform Generation
        if iteration > self.warmup_steps:
            predict = output['decoder_outputs'].squeeze()
            target = target.squeeze()

            #print ('---predict---',predict.shape)
            #print ('---target---',target.shape)


            # MR-STFT Loss
            stft_loss = self.stft_criterion(predict, target)
            if isinstance(stft_loss, dict):
                stft_loss_sum = 0
                for name, term in stft_loss.items():
                    stft_loss_sum = stft_loss_sum + term
                    losses[name] = term.item()
                stft_loss = stft_loss_sum
            losses['stft_loss'] = stft_loss.item()
            g_loss = g_loss + self.lambda_stft * stft_loss

            # Train discriminator
            fake_scores, fake_feats = self.model.discriminator(predict.detach())
            real_scores, real_feats = self.model.discriminator(target)

            d_loss_fake_list, d_loss_real_list = [], []
            for (real_score, fake_score) in zip(real_scores, fake_scores):
                d_loss_real_list.append(self.criterion(real_score, torch.ones_like(real_score)))
                d_loss_fake_list.append(self.criterion(fake_score, torch.zeros_like(fake_score)))
            d_loss_real = sum(d_loss_real_list)
            d_loss_fake = sum(d_loss_fake_list)
            d_loss = d_loss_real + d_loss_fake

            losses['d_loss_real'] = d_loss_real.item()
            losses['d_loss_fake'] = d_loss_fake.item()
            losses['d_loss'] = d_loss.item()

            self.optimizer.zero_grad(['discriminator'])
            d_loss.backward()
            self.optimizer.step(['discriminator'])
        
            # Train Generator
            fake_scores, fake_feats = self.model.discriminator(predict)
            real_scores, real_feats = self.model.discriminator(target)
            
            adv_loss_list = []
            feat_loss_list = []
            for fake_score in fake_scores:
                adv_loss_list.append(self.criterion(fake_score, torch.ones_like(fake_score)))
            for i in range(len(fake_feats)):
                for j in range(len(fake_feats[i])):
                    feat_loss_list.append(self.l1_loss(fake_feats[i][j], real_feats[i][j]))
            adv_loss = sum(adv_loss_list) # / len(adv_loss_list)
            feat_loss = sum(feat_loss_list) # / len(feat_loss_list)
            adv_loss = adv_loss + feat_loss * (
                self.lambda_fm if self.lambda_fm != 'auto' else
                (g_loss / feat_loss).detach())
            g_loss = g_loss + adv_loss

            losses['fm_loss'] = feat_loss.item()
            losses['adv_loss'] = adv_loss.item()
            losses['g_loss'] = g_loss.item()

        self.optimizer.zero_grad(['autoencoder'])
        g_loss.backward()
        grad_norm = nn.utils.clip_grad_norm_(
            self.model.autoencoder.parameters(), self.grad_clip_thresh)
        self.optimizer.step(['autoencoder'])

        return {'loss': losses}

    def random_select(self, mel_length):
        frame_windows, sample_windows = [], []
        for i in range(mel_length.shape[0]):
            start = random.randrange(max(1, mel_length[i] - self.frame_lengths))
            end = start + self.frame_lengths
            frame_windows.append((start, end))
            start, end = start * self.frameshift, end * self.frameshift
            sample_windows.append((start, end))
        return frame_windows, sample_windows


class PredictorTrainer(BaseTrainer):
    def __init__(self, config, model, num_gpus=1, rank=0,
                 grad_clip_thresh=1.0,
                 eval_inteval_iters=1000,
                 training_methods=['mse'],
                 loss_weights=[1.0],
                 lambda_dur=1.0,
                 ):
        super().__init__(config, model, num_gpus, rank)
        self.training_methods = training_methods
        self.loss_weights = loss_weights
        self.grad_clip_thresh = grad_clip_thresh
        self.eval_inteval_iters = eval_inteval_iters
        self.dur_loss = DurationLoss(lambda_dur)

    def train_step(self, batch, iteration):
        # Init loss
        losses = {'total_loss': 0}

        # Extract embedding features
        if not hasattr(self, 'autoencoder'):
            self.build_autoencoder()

        self.autoencoder.eval()
        self.autoencoder.require_grad = False
        with torch.no_grad():
            quantizer_states = self.autoencoder.analysis(
                batch.pop('mel'),
                batch.pop('mel_length').int()
            )
            batch['feat'] = quantizer_states['quantizer_outputs']
            batch['feat_length'] = quantizer_states['quantizer_lengths']

        # Model forward
        output = self.model.predictor(**batch)
        
        # Embedding loss
        pred = output['feat']
        length = output['feat_length']
        embedding_loss = self.autoencoder.compute_embedding_loss(
            pred, length, quantizer_states,
            methods=self.training_methods,
            loss_weights=self.loss_weights)
        losses['total_loss'] += embedding_loss.pop('total_loss')
        losses.update(embedding_loss)

        # FastSpeech loss
        dur_loss = self.dur_loss(output, batch)
        losses['total_loss'] += dur_loss.pop('total_loss')
        losses.update(dur_loss)

        # Update parameters
        self.optimizer.zero_grad(['predictor'])
        losses['total_loss'].backward()
        if self.grad_clip_thresh is not None:
            grad_norm = nn.utils.clip_grad_norm_(
                self.model.predictor.parameters(),
                self.grad_clip_thresh)
            losses['grad_norm'] = grad_norm
        self.optimizer.step(['predictor'])

        # Write logs
        log = {'loss': losses}
        
        return log

    def build_autoencoder(self):
        checkpoint = self.config.task.autoencoder._checkpoint
        config = self.config.task.autoencoder._config \
            if hasattr(self.config.task.autoencoder, '_config') \
            else None
        self.autoencoder = load_model('autoencoder', checkpoint, config)
        if torch.cuda.is_available():
            self.autoencoder = self.autoencoder.cuda()
