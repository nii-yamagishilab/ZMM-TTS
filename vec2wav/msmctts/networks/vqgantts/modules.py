from torch import nn
from torch.nn import functional as F

import numpy as np
import torch

from msmctts.utils.utils import get_mask_from_lengths


class Quantize(nn.Module):
    def __init__(self, embed_dim, n_embed, decay=0.99, eps=1e-5):
        super().__init__()

        self.dim = embed_dim
        self.n_embed = n_embed
        self.decay = decay
        self.eps = eps

        embed = torch.randn(embed_dim, n_embed)
        self.register_buffer("embed", embed)
        self.register_buffer("cluster_size", torch.zeros(n_embed))
        self.register_buffer("embed_avg", embed.clone())

    def forward(self, input, input_length=None, update=True, sort=False):
        Test_code = False
        if Test_code:
           embed_ind = input
           quantize = self.embed_code(embed_ind)
        else:
            flatten = input.reshape(-1, self.dim)
            dist = (
            flatten.pow(2).sum(1, keepdim=True)
            - 2 * flatten @ self.embed
            + self.embed.pow(2).sum(0, keepdim=True)
            )
            _, embed_ind = (-dist).max(1)
            embed_ind = embed_ind.view(*input.shape[:-1])
            #print (embed_ind.shape)
            quantize = self.embed_code(embed_ind)

        if self.training and update:
            embed_onehot = F.one_hot(
                embed_ind, self.n_embed).type(flatten.dtype)
            embed_onehot = torch.cat([embed_onehot[i, : int(input_length[i])]
                                     for i in range(input.shape[0])], dim=0)
            flatten = torch.cat([input[i, : int(input_length[i])]
                                for i in range(input.shape[0])], dim=0)

            embed_onehot_sum = embed_onehot.sum(0)
            embed_sum = flatten.transpose(0, 1) @ embed_onehot

            self.cluster_size.data.mul_(self.decay).add_(
                embed_onehot_sum, alpha=1 - self.decay
            )
            self.embed_avg.data.mul_(self.decay).add_(
                embed_sum, alpha=1 - self.decay)
            n = self.cluster_size.sum()
            cluster_size = (
                (self.cluster_size + self.eps) /
                (n + self.n_embed * self.eps) * n
            )
            embed_normalized = self.embed_avg / cluster_size.unsqueeze(0)
            self.embed.data.copy_(embed_normalized)

        if Test_code:
           diff=quantize
        else:
           diff = (quantize.detach() - input).pow(2)
           quantize = input + (quantize - input).detach()

        if sort:
            _, embed_ind = (-dist).sort(dim=1, descending=True)
            embed_ind = embed_ind.view(
                *(list(input.shape[:-1]) + [self.n_embed]))

        return quantize, diff, embed_ind

    def embed_code(self, embed_id):
        return F.embedding(embed_id, self.embed.transpose(0, 1))

    def sample(self, shape, prob=None, argmax=True):
        if prob is not None:
            prior_prob = self.cluster_size.view(1, -1).repeat(
                (shape[0] * shape[1], 1)).to(
                self.embed.device) / sum(self.cluster_size)
            indices = torch.argmax(prob, dim=-1)
        else:
            prob = self.cluster_size.view(1, -1).repeat(
                (shape[0] * shape[1], 1)).to(
                self.embed.device)
            indices = torch.multinomial(prob, 1).view(shape[0], shape[1])
        quant = torch.embedding(self.embed.T, indices)
        return quant, None, indices
    
    def compute_triple_loss(self, prd_quant, trg_quant,
                            reduction='mean',
                            margin=1e-6,
                            adaptive_margin=False):
        B, T, D = prd_quant.shape
        flatten = prd_quant.reshape(-1, self.dim)
        dist = (
            flatten.pow(2).sum(1, keepdim=True)
            - 2 * flatten @ self.embed
            + self.embed.pow(2).sum(0, keepdim=True)
        ).reshape(B, T, -1)

        pos_loss = F.mse_loss(prd_quant, self.embed_code(trg_quant),
                              reduction='none').sum(-1)

        # Unmasked version
        # triple_loss = torch.clamp(pos_loss.unsqueeze(-1) + margin - dist, min=0)
        # triple_loss = triple_loss / self.dim

        # Masked version
        triple_loss = pos_loss.unsqueeze(-1) - dist
        mask = (triple_loss != 0)
        triple_loss = torch.clamp(triple_loss + margin, min=0)
        triple_loss = mask * (triple_loss / self.dim)

        if reduction == 'mean':
            triple_loss = triple_loss.mean(-1)
        elif reduction == 'sum':
            triple_loss = triple_loss.sum(-1)

        return triple_loss


class MultiHeadQuantize(nn.Module):
    def __init__(self, embed_dim, n_embed, n_head,
                 decay=0.99, eps=1e-5):
        super().__init__()

        self.dim = embed_dim
        self.n_embed = n_embed
        self.n_head = n_head
        self.decay = decay
        self.eps = eps
        assert embed_dim % n_head == 0

        sub_dim = embed_dim // n_head
        self.quantizers = nn.ModuleList([
            Quantize(sub_dim, n_embed, decay, eps)
            for _ in range(n_head)
        ])

    def forward(self, input, input_length=None, update=True, sort=False):
        heads = torch.chunk(input, self.n_head, dim=-1)

        quants, diffs, inds = [], [], []
        for head, quantizer in zip(heads, self.quantizers):
            quant, diff, ind = quantizer(
                head, input_length, update, sort)
            quants.append(quant)
            diffs.append(diff)
            inds.append(ind)
        quant = torch.cat(quants, dim=-1)
        diff = sum(diffs) / len(diffs)
        ind = torch.stack(inds, dim=-1)

        return quant, diff, ind

    def compute_triple_loss(self, prd_quant, trg_quant,
                            reduction='mean',
                            margin=1e-6,
                            adaptive_margin=False):
        prd_quants = torch.chunk(prd_quant, self.n_head, dim=-1)
        trg_quants = torch.chunk(trg_quant, self.n_head, dim=-1)

        triple_loss = []
        for quantizer, prd_quant, trg_quant in zip(
                self.quantizers, prd_quants, trg_quants):
            assert trg_quant.shape[-1] == 1
            trg_quant = trg_quant.squeeze(-1)
            loss = quantizer.compute_triple_loss(
                prd_quant, trg_quant, reduction, margin, adaptive_margin)
            triple_loss.append(loss)

        return sum(triple_loss) / len(triple_loss)


@torch.jit.script
def fused_add_tanh_sigmoid_multiply(input_a, input_b, n_channels):
    n_channels_int = n_channels[0]
    in_act = input_a + input_b
    t_act = torch.tanh(in_act[:, :n_channels_int, :])
    s_act = torch.sigmoid(in_act[:, n_channels_int:, :])
    acts = t_act * s_act
    return acts


class ResStack(torch.nn.Module):
    def __init__(self, hidden_channels, kernel_size, dilation_rate, n_layers, gin_channels=0, p_dropout=0.1):
        super(ResStack, self).__init__()
        assert(kernel_size % 2 == 1)
        self.hidden_channels = hidden_channels
        self.kernel_size = kernel_size,
        self.dilation_rate = dilation_rate
        self.n_layers = n_layers
        self.gin_channels = gin_channels
        self.p_dropout = p_dropout

        self.in_layers = torch.nn.ModuleList()
        self.res_skip_layers = torch.nn.ModuleList()
        self.drop = nn.Dropout(p_dropout)

        if gin_channels != 0:
            cond_layer = torch.nn.Conv1d(
                gin_channels, 2*hidden_channels*n_layers, 1)
            self.cond_layer = torch.nn.utils.weight_norm(
                cond_layer, name='weight')

        for i in range(n_layers):
            dilation = dilation_rate ** i
            padding = int((kernel_size * dilation - dilation) / 2)
            in_layer = torch.nn.Conv1d(hidden_channels, 2*hidden_channels, kernel_size,
                                       dilation=dilation, padding=padding)
            in_layer = torch.nn.utils.weight_norm(in_layer, name='weight')
            self.in_layers.append(in_layer)

            # last one is not necessary
            if i < n_layers - 1:
                res_skip_channels = 2 * hidden_channels
            else:
                res_skip_channels = hidden_channels

            res_skip_layer = torch.nn.Conv1d(
                hidden_channels, res_skip_channels, 1)
            res_skip_layer = torch.nn.utils.weight_norm(
                res_skip_layer, name='weight')
            self.res_skip_layers.append(res_skip_layer)

    def forward(self, x, x_mask, g=None, **kwargs):
        output = torch.zeros_like(x)
        n_channels_tensor = torch.IntTensor([self.hidden_channels])

        if g is not None:
            g = self.cond_layer(g)

        for i in range(self.n_layers):
            x_in = self.in_layers[i](x)
            if g is not None:
                cond_offset = i * 2 * self.hidden_channels
                g_l = g[:, cond_offset:cond_offset+2*self.hidden_channels, :]
            else:
                g_l = torch.zeros_like(x_in)

            acts = fused_add_tanh_sigmoid_multiply(
                x_in,
                g_l,
                n_channels_tensor)
            acts = self.drop(acts)

            res_skip_acts = self.res_skip_layers[i](acts)
            if i < self.n_layers - 1:
                res_acts = res_skip_acts[:, :self.hidden_channels, :]
                x = (x + res_acts) * x_mask
                output = output + res_skip_acts[:, self.hidden_channels:, :]
            else:
                output = output + res_skip_acts
        return output * x_mask

    def remove_weight_norm(self):
        if self.gin_channels != 0:
            torch.nn.utils.remove_weight_norm(self.cond_layer)
        for l in self.in_layers:
            torch.nn.utils.remove_weight_norm(l)
        for l in self.res_skip_layers:
            torch.nn.utils.remove_weight_norm(l)


class Encoder(nn.Module):
    def __init__(self,
                in_channels,
                out_channels,
                hidden_channels,
                kernel_size=5,
                dilation_rate=1,
                n_layers=16):
        super().__init__()
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.hidden_channels = hidden_channels
        self.kernel_size = kernel_size
        self.dilation_rate = dilation_rate
        self.n_layers = n_layers

        self.pre = nn.Conv1d(in_channels, hidden_channels, 1)
        self.enc = ResStack(hidden_channels, kernel_size,
                            dilation_rate, n_layers)
        self.proj = nn.Conv1d(hidden_channels, out_channels, 1)

    def forward(self, x, x_lengths):
        x = x.transpose(1, 2)
        x_mask = ~get_mask_from_lengths(x_lengths).unsqueeze(1)
        x = self.pre(x) * x_mask
        h = self.enc(x, x_mask)
        x = self.proj(h) * x_mask
        return x.transpose(1, 2), h.transpose(1, 2)
