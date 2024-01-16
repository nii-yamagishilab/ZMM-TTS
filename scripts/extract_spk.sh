#!/bin/bash
#SBATCH --job-name=spk
#SBATCH --out=spk.log
#SBATCH --error=spk.error
#SBATCH --time=24:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate vec2wav2
fi

python prepare_data/extract_spk_emb.py