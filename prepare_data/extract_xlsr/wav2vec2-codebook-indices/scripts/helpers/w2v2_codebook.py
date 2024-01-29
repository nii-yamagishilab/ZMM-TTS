import torch
import torchaudio
import warnings
import os
import numpy as np
from dataclasses import dataclass
from transformers import Wav2Vec2FeatureExtractor
from transformers.models.wav2vec2.modeling_wav2vec2 import (
    Wav2Vec2Config,
    Wav2Vec2ForPreTrainingOutput,
    Wav2Vec2GumbelVectorQuantizer,
    Wav2Vec2ForPreTraining
)
from typing import Optional, Tuple, Union

# Extend class, modify forward() to return 'codebook_pairs'
class Wav2Vec2GumbelVectorQuantizerWithCodebookIndices(Wav2Vec2GumbelVectorQuantizer):

    def forward(self, hidden_states, mask_time_indices=None):
        batch_size, sequence_length, hidden_size = hidden_states.shape

        # project to codevector dim
        hidden_states = self.weight_proj(hidden_states)
        hidden_states = hidden_states.view(batch_size * sequence_length * self.num_groups, -1)

        # take argmax in non-differentiable way
        # comptute hard codevector distribution (one hot)
        codevector_idx = hidden_states.argmax(dim=-1)
        codevector_probs = hidden_states.new_zeros(*hidden_states.shape).scatter_(
            -1, codevector_idx.view(-1, 1), 1.0
        )
        codevector_probs = codevector_probs.view(batch_size * sequence_length, self.num_groups, -1)
        codebook_pairs = torch.argmax(codevector_probs, dim=-1)

        perplexity = self._compute_perplexity(codevector_probs, mask_time_indices)
        print ('0000000',codevector_probs.shape)
        codevector_probs = codevector_probs.view(batch_size * sequence_length, -1)
        print ('-------view-------------',codevector_probs.shape)
        #print ('0000000',codevector_probs)
        print ('1111111',self.codevectors.shape)
        #print (codevector_probs.unsqueeze(-1).shape)
        #print (codevector_probs.unsqueeze(-1))
        # use probs to retrieve codevectors
        codevectors_per_group = codevector_probs.unsqueeze(-1) * self.codevectors
        print('2222222222222222222',codevectors_per_group.shape)
        #print ('----self.codevectors---',self.codevectors.shape)
        '''
        codevectors_book1 = self.codevectors[:,0:320,:]
        codevectors_book2 = self.codevectors[:,320:,:]
        codevectors_book1= codevectors_book1.squeeze(0).cpu().numpy()
        codevectors_book2= codevectors_book2.squeeze(0).cpu().numpy()
        np.save('code_book1.npy', codevectors_book1)
        np.save('code_book2.npy', codevectors_book2)
        '''
        #print ('----self.codevectors_pergroup---',codevectors_per_group.shape)
        codevectors = codevectors_per_group.view(batch_size * sequence_length, self.num_groups, self.num_vars, -1)
        print ('----codevectors [10] ------',codevectors.shape)
        print ('----codevectors [19] ------',codevectors.sum(-2).shape)
        codevectors = codevectors.sum(-2).view(batch_size, sequence_length, -1)
        print ('----codevectors [11] ------',codevectors.shape)
        return codevectors, perplexity, codebook_pairs

# Extend output class to have place for 'codebook_pairs'
@dataclass
class Wav2Vec2ForPreTrainingOutputWithCodebookIndices(Wav2Vec2ForPreTrainingOutput):
    codebook_pairs: Optional[Tuple[torch.FloatTensor]] = None

# Extend class, modify forward() to return 'codebook_pairs'
class Wav2Vec2ForPreTrainingWithCodebookIndices(Wav2Vec2ForPreTraining):
    def __init__(self, config: Wav2Vec2Config):
        super().__init__(config)

        self.quantizer = Wav2Vec2GumbelVectorQuantizerWithCodebookIndices(config)

    def forward(
        self,
        input_values: Optional[torch.Tensor],
        attention_mask: Optional[torch.Tensor] = None,
        mask_time_indices: Optional[torch.BoolTensor] = None,
        output_attentions: Optional[bool] = None,
        output_hidden_states: Optional[bool] = None,
        return_dict: Optional[bool] = None,
    ) -> Union[Tuple, Wav2Vec2ForPreTrainingOutput]:

        return_dict = return_dict if return_dict is not None else self.config.use_return_dict

        if mask_time_indices is not None:
            mask_time_indices = mask_time_indices.to(torch.bool)

        outputs = self.wav2vec2(
            input_values,
            attention_mask=attention_mask,
            output_attentions=output_attentions,
            output_hidden_states=output_hidden_states,
            mask_time_indices=mask_time_indices,
            return_dict=return_dict,
        )

        # 1. project all transformed features (including masked) to final vq dim
        transformer_features = self.project_hid(outputs[0])

        # 2. quantize all (unmasked) extracted features and project to final vq dim
        extract_features = self.dropout_features(outputs[1])

        # Modified quantizer provides 'codebook_pairs' as third item in returned tuple
        quantized_features, codevector_perplexity, codebook_pairs = self.quantizer(
            extract_features, mask_time_indices=mask_time_indices
        )
        org_quan = quantized_features
        #print ('----before-----',quantized_features,quantized_features.shape)
        quantized_features = self.project_q(quantized_features)
        #print ('----out-----',quantized_features, quantized_features.shape)
        loss = contrastive_loss = diversity_loss = None

        if not return_dict:
            if loss is not None:
                return (loss, transformer_features, quantized_features, codevector_perplexity) + outputs[2:]
            return (transformer_features, quantized_features, codevector_perplexity) + outputs[2:]

        return Wav2Vec2ForPreTrainingOutputWithCodebookIndices(
            loss=loss,
            projected_states=transformer_features,
            projected_quantized_states=quantized_features,
            codevector_perplexity=codevector_perplexity,
            hidden_states=outputs.hidden_states,
            attentions=outputs.attentions,
            contrastive_loss=contrastive_loss,
            diversity_loss=diversity_loss,
            codebook_pairs=codebook_pairs
        ), org_quan

def get_codebook_indices(wav_file):
    # Catch HF gradient_checkpointing deprecation warning
    warnings.filterwarnings(action='ignore', category=UserWarning, module=r'.*configuration_utils')

    feature_extractor = Wav2Vec2FeatureExtractor.from_pretrained("facebook/wav2vec2-large-960h")
    model = Wav2Vec2ForPreTrainingWithCodebookIndices.from_pretrained("facebook/wav2vec2-large-960h")

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.to(device)

    wav, _ = torchaudio.load(wav_file)
    wav = wav.squeeze()

    input_values = feature_extractor(wav, sampling_rate=16_000, return_tensors="pt").input_values  # Batch size 1

    with torch.no_grad():
        outputs, org_quan = model(input_values.to(device), output_hidden_states=True, return_dict=True)

    return outputs.codebook_pairs.cpu().numpy(), org_quan, org_quan.shape




def get_codebook_indices_lists(data_dir):
    # Catch HF gradient_checkpointing deprecation warning
    #wav2vec2-large-960h

    warnings.filterwarnings(action='ignore', category=UserWarning, module=r'.*configuration_utils')

    feature_extractor = Wav2Vec2FeatureExtractor.from_pretrained("facebook/wav2vec2-large-960h")

    

    model = Wav2Vec2ForPreTrainingWithCodebookIndices.from_pretrained("facebook/wav2vec2-large-960h")

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model.to(device)

    filelist = os.listdir(data_dir)
    for wav_name in filelist:
      if wav_name.endswith('wav'):
        wav_file = os.path.join(data_dir,wav_name)
        wav, sr = torchaudio.load(wav_file)
        assert sr == 16000
        wav = wav.squeeze()

        input_values = feature_extractor(wav, sampling_rate=16000, return_tensors="pt").input_values  # Batch size 1

        with torch.no_grad():
           outputs, org_quan = model(input_values.to(device), output_hidden_states=True, return_dict=True)
        org_quan = org_quan.squeeze(0).cpu().numpy()
        index = outputs.codebook_pairs.cpu().numpy()
        #print (org_quan.dtype)
        #print (outputs.codebook_pairs.cpu().numpy())
        #print (outputs.codebook_pairs.cpu().shape)
        np.save(os.path.join('Dataset/preprocessed_data/MM6/DiscreteRp',wav_name[:-3]+'npy'), org_quan)
        np.save(os.path.join('Dataset/preprocessed_data/MM6/CodeIndex',wav_name[:-3]+'npy'), index)
        #torch.save(org_quan, os.path.join('/home/others/v-cheng-gong/Low_resources_languages/wav2vec2tf/wav2vec2-codebook-indices/LJ_quantization',wav_name[:-3]+'pt'))
        #print (wav_name, org_quan.shape)
