from torch.nn.utils.rnn import pad_sequence

import numpy as np
import torch

from msmctts.utils.utils import align_features
from msmctts.datasets.base_dataset import BaseDataset


class MelDataset(BaseDataset):
    
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    
    def parse_case(self, index):
        feat_dict = super().parse_case(index)
        # align different sequences and select segments randomly
        seq_dict = {k: v for k, v in feat_dict.items()
                    if k in self.frameshift and self.frameshift[k] > 0}
        seq_dict = align_features(seq_dict, self.frameshift)
        feat_dict.update(seq_dict)
    
        return feat_dict

    def collate_fn(self, batch):
        feat_dict = {name: [torch.from_numpy(feat[name])
                     if isinstance(feat[name], np.ndarray) else feat[name]
                     for feat in batch] for name in batch[0].keys()}
        
        # Get sorted indices in descending
        sorted_lengths, ids = torch.sort(
            torch.LongTensor([x.shape[0] for x in feat_dict['mel']]),
            dim=0, descending=True)
        
        # Re-sort the batch according to ids
        output_dict = {}
        for k, v in feat_dict.items():
            v = [v[i] for i in ids]
    
            if k in ['dur', 'npw']:
                output_dict[k + '_length'] = torch.Tensor(
                    [x.shape[0] for x in v]).int().to(sorted_lengths.device)
                v = [x.squeeze(-1) if len(x.shape) == 2 else x for x in v]

            if isinstance(v[0], torch.Tensor) and k!='speaker':
                #print ('kkk',k)
                v = pad_sequence(v,
                        batch_first=True, padding_value=self.padding_value[k]) \
                    if len(v[0].shape) >= 1 \
                    else torch.stack(v)
            output_dict[k] = v# self.list_to_tensor(v, self.padding_value[k])
        
        output_dict['mel_length'] = sorted_lengths
        if 'wav' in output_dict:
            output_dict['wav_length'] = sorted_lengths * self.frameshift['mel']
        #print ('-----')
        #print (output_dict)
        output_dict['language']=torch.Tensor(output_dict['language'])
        #print (output_dict['language'])
        #print (output_dict['speaker'][0].shape)
        #print ('--------',len(output_dict['speaker']))
        output_dict['speaker']= torch.stack(output_dict['speaker'])
        #print (output_dict['speaker'].shape)       
        return output_dict

    def list_to_tensor(self, x, padding):
        if isinstance(x[0], (list, tuple)):
            return [self.list_to_tensor(x, padding) for l in zip(*x)]
        elif isinstance(x[0], torch.Tensor):
            x = pad_sequence(x,
                    batch_first=True, padding_value=padding) \
                if len(x[0].shape) >= 1 \
                else torch.stack(x)
        return x
