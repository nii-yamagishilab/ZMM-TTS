# ZMM-TTS: Zero-shot Multilingual and Multispeaker Speech Synthesis Conditioned on Self-supervised Discrete Speech Representations

<a href='https://gongchenghhu.github.io/TASLP-demo/'><img src='https://img.shields.io/badge/Demo-Page-blue'></a>  <a href='https://arxiv.org/abs/2312.14398'><img src='https://img.shields.io/badge/Paper-Arxiv-red'></a>

## Introduction
This is the code for the ZMM-TTS submitted to the [IEEE TASLP](https://arxiv.org/abs/2312.14398). This paper presents ZMM-TTS, a multilingual and multispeaker framework utilizing quantized latent speech representations from a large-scale, pre-trained, self-supervised model. Our paper is the first to incorporate the representations from text-based and speech-based self-supervised learning models into multilingual speech synthesis tasks. We conducted comprehensive subjective and objective evaluations through a series of experiments. Our model has been proven effective in terms of speech naturalness and similarity for both seen and unseen speakers in six high-resource languages. We also tested the efficiency of our method on two hypothetical low-resource languages. The results are promising, indicating that our proposed approach can synthesize audio that is intelligible and has a high degree of similarity to the target speaker's voice, even without any training data for the new, unseen language. 

<br>
<p align="center">
    <img src="images/overview.png" width="95%"> <br>
    Overview
</p>
<p align="center">
    <img src="images/speechtokenizer_framework.jpg" width="95%"> <br>
    The SpeechTokenizer framework.
</p>
<be>

Welcome to try our code and pre-trained model on different languages!

## Release 
- [20/01] 🔥 We released code and model pre-trained on 6 language (English, French, German, Portuguese, Spanish and Swedish) public datasets.

## Samples
Samples are provided on [our demo page](https://gongchenghhu.github.io/TASLP-demo/).

## Installation

ZMM-TTS requires Python>=3.8, and a reasonly recent version of PyTorch.
To install ZMM-TTS and make a quick synthesis, you can run from this repository:
```bash
git clone https://github.com/nii-yamagishilab-visitors/ZMM-TTS.git

cd ZMM-TTS
pip3 install -r requirements.txt
#In addition, you may need to install these libraries to support full functionality.
pip install transformers  #For support XLSR-53 and XphoneBERT model.
pip install speechbrain   #For extracting speaker embedding.
```
If you want to try IPA representations, you need to install [Epitran](https://github.com/dmort27/epitran).
## Usage
### Model storage
| Model| Dataset |Discription|
|:----|:----:|:----|
|[speechtokenizer_hubert_avg](https://huggingface.co/fnlp/SpeechTokenizer/tree/main/speechtokenizer_hubert_avg)|LibriSpeech|Adopt average representation across all HuBERT layers as semantic teacher |
### load model
```python
from speechtokenizer import SpeechTokenizer

config_path = '/path/config.json'
ckpt_path = '/path/SpeechTokenizer.pt'
model = SpeechTokenizer.load_from_checkpoint(config_path, ckpt_path)
model.eval()
```
### Extracting discrete representations
```python
import torchaudio
import torch

# Load and pre-process speech waveform
wav, sr = torchaudio.load('<SPEECH_FILE_PATH>')

# monophonic checking
if wav.shape(0) > 1:
    wav = wav[:1,;]

if sr != model.sample_rate:
    wav = torchaudio.functional.resample(wav, sr, model.sample_rate)

wav = wav.unsqueeze(0)

# Extract discrete codes from SpeechTokenizer
with torch.no_grad():
    codes = model.encode(wav) # codes: (n_q, B, T)

RVQ_1 = codes[:1, :, :] # Contain content info, can be considered as semantic tokens
RVQ_supplement = codes[1:, :, :] # Contain timbre info, complete info lost by the first quantizer
```

### Decoding discrete representations
```python
# Concatenating semantic tokens (RVQ_1) and supplementary timbre tokens and then decoding
wav = model.decode(torch.cat([RVQ_1, RVQ_supplement], axis=0))

# Decoding from RVQ-i:j tokens from the ith quantizers to the jth quantizers
wav = model.decode(codes[i: (j + 1)], st=i) 
```

## Citation
If you use this code or result in your paper, please cite our work as:
```Tex
@misc{zhang2023speechtokenizer,
      title={SpeechTokenizer: Unified Speech Tokenizer for Speech Language Models}, 
      author={Xin Zhang and Dong Zhang and Shimin Li and Yaqian Zhou and Xipeng Qiu},
      year={2023},
      eprint={2308.16692},
      archivePrefix={arXiv},
      primaryClass={cs.CL}
}
```
## License
The code in this repository is released under the Apache 2.0 license as found in the
[LICENSE](LICENSE) file.
