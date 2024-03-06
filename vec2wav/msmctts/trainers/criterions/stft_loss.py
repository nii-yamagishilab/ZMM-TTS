from librosa.filters import mel as librosa_mel_fn

import torch
import torch.nn as nn
import torch.nn.functional as F


from msmctts.utils.audio import MelScale


def stft(x, fft_size, hop_size, win_size, window):
    """Perform STFT and convert to magnitude spectrogram.

    Args:
        x: Input signal tensor (B, T).

    Returns:
        Tensor: Magnitude spectrogram (B, T, fft_size // 2 + 1).

    """
    x_stft = torch.stft(x, fft_size, hop_size, win_size, window)
    real = x_stft[..., 0]
    imag = x_stft[..., 1]
    outputs = torch.clamp(real ** 2 + imag ** 2, min=1e-7).transpose(2, 1)
    outputs = torch.sqrt(outputs)

    return outputs


class SpectralConvergence(nn.Module):
    def __init__(self):
        """Initilize spectral convergence loss module."""
        super(SpectralConvergence, self).__init__()

    def forward(self, predicts_mag, targets_mag):
        x = torch.norm(targets_mag - predicts_mag, p='fro')
        y = torch.norm(targets_mag, p='fro')

        return x / y 


class LogSTFTMagnitude(nn.Module):
    def __init__(self):
        super(LogSTFTMagnitude, self).__init__()
    
    def forward(self, predicts_mag, targets_mag):
        log_predicts_mag = torch.log(torch.clamp(predicts_mag, min=1e-5, max=10))
        log_targets_mag = torch.log(torch.clamp(targets_mag, min=1e-5, max=10))

        outputs = F.l1_loss(log_predicts_mag, log_targets_mag)

        return outputs


class MelLoss(nn.Module):
    def __init__(self, fft_size, hop_size, win_size, sample_rate, num_mels):
        super(MelLoss, self).__init__()
        
        self.sample_rate = sample_rate
        self.fft_size = fft_size
        self.hop_size = hop_size
        self.win_size = win_size
        self.num_mels = num_mels
        self.fmin = 0
        self.fmax = sample_rate // 2
        self.window = torch.hann_window(win_size)
        self.mel_scale = MelScale(num_mels, sample_rate)
        self.mel_basis = {}
        self.hann_window = {}
        
    def forward(self, predicts, targets):
        #print ('mel_spectrogram predicts',predicts.shape)
        #print ('mel_spectrogram targets',targets.shape)
        predicts_mel = self.mel_spectrogram(predicts)
        targets_mel = self.mel_spectrogram(targets)
        mel_loss = F.l1_loss(predicts_mel, targets_mel)

        return mel_loss

    def mel_spectrogram(self, y, center=False):
        if torch.min(y) < -1.:
            print('min value is ', torch.min(y))
        if torch.max(y) > 1.:
            print('max value is ', torch.max(y))

        if self.fmax not in self.mel_basis:
            mel = librosa_mel_fn(self.sample_rate, self.fft_size, self.num_mels, self.fmin, self.fmax)
            self.mel_basis[str(self.fmax) + '_' + str(y.device)] = torch.from_numpy(mel).float().to(y.device)
            self.hann_window[str(y.device)] = torch.hann_window(self.win_size).to(y.device)

        y = torch.nn.functional.pad(y.unsqueeze(1),
            (int((self.fft_size - self.hop_size) / 2),
             int((self.fft_size - self.hop_size) / 2)),
            mode='reflect')
        y = y.squeeze(1)

        spec = torch.stft(y, self.fft_size,
                          hop_length=self.hop_size,
                          win_length=self.win_size,
                          window=self.hann_window[str(y.device)],
                          center=center, pad_mode='reflect',
                          normalized=False, onesided=True)

        spec = torch.sqrt(spec.pow(2).sum(-1) + 1e-9)

        spec = torch.matmul(self.mel_basis[str(self.fmax) + '_' + str(y.device)], spec)
        spec = self.spectral_normalize_torch(spec)

        return spec

    def spectral_normalize_torch(self, magnitudes):
        output = self.dynamic_range_compression_torch(magnitudes)
        return output

    def dynamic_range_compression_torch(self, x, C=1, clip_val=1e-5):
        return torch.log(torch.clamp(x, min=clip_val) * C)


class STFTLoss(nn.Module):
    def __init__(self, fft_size, hop_size, win_size,
                 mel_scale=False, sample_rate=16000):
        super(STFTLoss, self).__init__()
        
        self.fft_size = fft_size
        self.hop_size = hop_size
        self.win_size = win_size
        self.window = torch.hann_window(win_size)
        self.sc_loss = SpectralConvergence()
        self.mag_loss = LogSTFTMagnitude()
        if mel_scale:
            self.mel_scale = MelScale(fft_size // 2 + 1, sample_rate=sample_rate)

        
    def forward(self, predicts, targets):
        print ('---loss targets shapeL',targets.shape)
        print ('---loss predicts shapeL',predicts.shape)
        predicts_mag = stft(predicts, self.fft_size, self.hop_size, self.win_size, self.window.to(targets.device))
        targets_mag = stft(targets, self.fft_size, self.hop_size, self.win_size, self.window.to(targets.device))

        if hasattr(self, 'mel_scale'):
            predicts_mag = self.mel_scale(predicts_mag)
            targets_mag = self.mel_scale(targets_mag)

        sc_loss = self.sc_loss(predicts_mag, targets_mag)
        mag_loss = self.mag_loss(predicts_mag, targets_mag)

        return sc_loss, mag_loss


class MultiResolutionSTFTLoss(nn.Module):
    def __init__(self,
                 fft_sizes=[1024, 2048, 512],
                 win_sizes=[400, 800, 200],
                 hop_sizes=[80, 160, 40],
                 mel_scale=False,
                 sample_rate=16000):
        super(MultiResolutionSTFTLoss, self).__init__()

        self.loss_layers = torch.nn.ModuleList()
        for (fft_size, win_size, hop_size) in zip(fft_sizes, win_sizes, hop_sizes):
            self.loss_layers.append(STFTLoss(fft_size, hop_size, win_size, mel_scale, sample_rate))
            
    def forward(self, fake_signals, true_signals):
        sc_losses = []
        mag_losses = []
        for layer in self.loss_layers:
            sc_loss, mag_loss = layer(fake_signals, true_signals)
            sc_losses.append(sc_loss)
            mag_losses.append(mag_loss)
        
        sc_loss = sum(sc_losses) / len(sc_losses)
        mag_loss = sum(mag_losses) / len(mag_losses)

        return {
            'sc_loss': sc_loss,
            'mag_loss': mag_loss,
        }
