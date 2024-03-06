from torch.nn import Conv1d, AvgPool1d, Conv2d
from torch.nn.utils import weight_norm, spectral_norm

import torch
import torch.nn.functional as F
import torch.nn as nn

from msmctts.utils.audio import MelScale, TorchSTFT
from .common import *


LRELU_SLOPE = 0.2


class DiscriminatorR(nn.Module):
    def __init__(self, in_channels, hidden_channels=512):
        super(DiscriminatorR, self).__init__()

        self.discriminator = nn.ModuleList()
        self.discriminator += [
            nn.Sequential(
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    in_channels, hidden_channels // 32,
                    kernel_size=(3, 3), stride=(1, 1)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels // 32, hidden_channels // 16,
                    kernel_size=(3, 3), stride=(2, 2)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels // 16, hidden_channels // 8,
                    kernel_size=(3, 3), stride=(1, 1)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels // 8, hidden_channels // 4,
                    kernel_size=(3, 3), stride=(2, 2)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels // 4, hidden_channels // 2,
                    kernel_size=(3, 3), stride=(1, 1)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels // 2, hidden_channels,
                    kernel_size=(3, 3), stride=(2, 2)))
            ),
            nn.Sequential(
                nn.LeakyReLU(0.2, True),
                nn.ReflectionPad2d((1, 1, 1, 1)),
                nn.utils.weight_norm(nn.Conv2d(
                    hidden_channels, 1,
                    kernel_size=(3, 3), stride=(1, 1)))
            )
        ]

    def forward(self, x):
        hiddens = []
        for layer in self.discriminator:
            x = layer(x)
            hiddens.append(x)
        return x, hiddens[: -1]


class MultiResolutionDiscriminator(nn.Module):
    def __init__(self,
                 hop_lengths=[15, 30, 50, 120, 240, 480],
                 hidden_channels=[128, 128, 256, 256, 512, 512],
                 domain='double', mel_scale=True, sample_rate=24000):
        super(MultiResolutionDiscriminator, self).__init__()
        
        self.stfts = nn.ModuleList([
            TorchSTFT(fft_size=x * 4, hop_size=x, win_size=x * 4,
                      normalized=True, domain=domain,
                      mel_scale=mel_scale, sample_rate=sample_rate)
            for x in hop_lengths])

        self.domain = domain
        if domain == 'double':
            self.discriminators = nn.ModuleList([
                DiscriminatorR(2, c)
                for x, c in zip(hop_lengths, hidden_channels)])
        else:
            self.discriminators = nn.ModuleList([
                DiscriminatorR(1, c)
                for x, c in zip(hop_lengths, hidden_channels)])

    def forward(self, x):
        scores, feats = list(), list()

        for stft, layer in zip(self.stfts, self.discriminators):
            mag, phase = stft.transform(x.squeeze())
            if self.domain == 'double':
                mag = torch.stack(torch.chunk(mag, 2, dim=1), dim=1)
            else:
                mag = mag.unsqueeze(1)

            score, feat = layer(mag)
            scores.append(score)
            feats.append(feat)
            
        return scores, feats


class DiscriminatorP(torch.nn.Module):
    def __init__(self, period, ch=32, max_ch=1024, kernel_size=5, stride=3, use_spectral_norm=False):
        super(DiscriminatorP, self).__init__()
        self.period = period
        norm_f = weight_norm if use_spectral_norm == False else spectral_norm

        ch1, ch2, ch3, ch4 = ch, ch * 4, min(max_ch, ch * 16), min(max_ch, ch * 32)
        self.convs = nn.ModuleList([
            norm_f(Conv2d(1, ch1, (kernel_size, 1), (stride, 1), padding=(get_padding(kernel_size, 1), 0))),
            norm_f(Conv2d(ch1, ch2, (kernel_size, 1), (stride, 1), padding=(get_padding(kernel_size, 1), 0))),
            norm_f(Conv2d(ch2, ch3, (kernel_size, 1), (stride, 1), padding=(get_padding(kernel_size, 1), 0))),
            norm_f(Conv2d(ch3, ch4, (kernel_size, 1), (stride, 1), padding=(get_padding(kernel_size, 1), 0))),
            norm_f(Conv2d(ch4, ch4, (5, 1), 1, padding=(2, 0))),
        ])
        self.conv_post = norm_f(Conv2d(ch4, 1, (3, 1), 1, padding=(1, 0)))

    def forward(self, x):
        fmap = []

        # 1d to 2d
        b, c, t = x.shape
        if t % self.period != 0: # pad first
            n_pad = self.period - (t % self.period)
            x = F.pad(x, (0, n_pad), "reflect")
            t = t + n_pad
        x = x.view(b, c, t // self.period, self.period)

        for l in self.convs:
            x = l(x)
            fmap.append(x)
            x = F.leaky_relu(x, LRELU_SLOPE)
        
        x = self.conv_post(x)
        x = torch.flatten(x, 1, -1)

        return x, fmap


class MultiPeriodDiscriminator(torch.nn.Module):
    def __init__(self, periods=[2, 3, 5, 7, 11], channels=32, max_channels=1024):
        super(MultiPeriodDiscriminator, self).__init__()
        self.discriminators = nn.ModuleList([
            DiscriminatorP(p, channels, max_channels) for p in periods
        ])

    def forward(self, y):
        outputs, fmaps = [], []
        for i, d in enumerate(self.discriminators):
            output, fmap = d(y)
            outputs.append(output)
            fmaps.append(fmap)

        return outputs, fmaps


class Discriminator(torch.nn.Module):
    def __init__(self, mrd_config, mpd_config):
        super(Discriminator, self).__init__()
        self.mrd = MultiResolutionDiscriminator(**mrd_config)
        self.mpd = MultiPeriodDiscriminator(**mpd_config)

    def forward(self, y):
        if len(y.shape) == 2:
            y = y.unsqueeze(1)

        mrd_outputs, mrd_fmaps = self.mrd(y)
        mpd_outputs, mpd_fmaps = self.mpd(y)

        outputs = mrd_outputs + mpd_outputs
        fmaps = mrd_fmaps + mpd_fmaps

        return outputs, fmaps
