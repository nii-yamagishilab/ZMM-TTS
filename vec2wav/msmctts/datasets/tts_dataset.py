from torch.nn.utils.rnn import pad_sequence

import numpy as np
import torch

from msmctts.datasets.base_dataset import BaseDataset
from msmctts.utils.utils import align_features


class TTSDataset(BaseDataset):
    """
    This is the main class that calculates the spectrogram and returns the
    spectrogram, audio pair.
    """
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    
    def parse_case(self, index):
        feat_dict = super().parse_case(index)
        
        # align different sequences and select segments randomly
        seq_dict = {k: v for k, v in feat_dict.items()
                    if k in self.frameshift and self.frameshift[k] > 0}
        seq_dict = align_features(seq_dict, self.frameshift)
        feat_dict.update(seq_dict)

        # Process Text (& durations)
        if len(feat_dict['text'].shape) == 2 and feat_dict['text'].shape[1] == 1:
            feat_dict['text'] = feat_dict['text'][:, 0]
        text_length = len(feat_dict['text'])
        
        if 'dur' in feat_dict:
            durs = feat_dict['dur']
            if len(durs.shape) == 2:
                durs = durs.squeeze(1)
            assert len(durs) == text_length, \
                f"{self.id_list[index]} : {len(durs)} v.s. {text_length}"

            if 'mel' in feat_dict:
                if feat_dict['mel'].shape[0] / sum(durs) > 100:
                    durs = durs * self.samplerate / self.frameshift['mel']
                    for i in range(len(durs)):
                        int_f = round(durs[i])
                        if i < len(durs) - 1:
                            durs[i + 1] += durs[i] - int_f
                        durs[i] = int_f

                shift_dur = feat_dict['mel'].shape[0] - sum(durs)
                assert shift_dur <= 5 and shift_dur >= -5, \
                    f"{self.id_list[index]}: {feat_dict['mel'].shape[0]} v.s. {sum(durs)}"
                durs[-1] += shift_dur

            feat_dict['dur'] = durs
        
        return feat_dict

    def collate_fn(self, batch):
        feat_dict = {name: [torch.from_numpy(feat[name])
                     if isinstance(feat[name], np.ndarray) else feat[name]
                     for feat in batch] for name in batch[0].keys()}
        
        # Get sorted indices of texts in descending
        sorted_lengths, ids = torch.sort(
            torch.LongTensor([x.shape[0] for x in feat_dict['text']]),
            dim=0, descending=True)
        
        # Re-sort the batch according to ids
        for k, v in feat_dict.items():
            v = [v[i] for i in ids]
            feat_dict.update({k: v})

        # Global features
        if 'speaker' in feat_dict:
            feat_dict['speaker'] = torch.Tensor(feat_dict['speaker'])

        # Text-Level features
        feat_dict['text_length'] = sorted_lengths
        for name in ['text', 'tone', 'dur']:
            if name not in feat_dict:
                continue
            feat = pad_sequence(feat_dict[name],
                                batch_first=True,
                                padding_value=self.padding_value[name])
            feat_dict[name] = feat

        # Audio-level Features
        for name in ['mel', 'wav', 'pitch', 'energy']:
            if name not in feat_dict:
                continue
            
            if name in ['mel', 'wav']:
                length = [x.shape[0] for x in feat_dict[name]]
                feat_dict[name + '_length'] = torch.Tensor(length)
            
            feat_dict[name] = pad_sequence(feat_dict[name],
                                           batch_first=True,
                                           padding_value=self.padding_value[name])

        return feat_dict
