#!/bin/bash
#SBATCH --job-name=wav2vec2_ex
#SBATCH --out=wav2vec2d.log
#SBATCH --error=wav2vec2d.error
#SBATCH --time=24:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate wav2vec2tf
fi

python prepare_data/extract_xlsr/wav2vec2-codebook-indices/scripts/get_codebook_indices.py
