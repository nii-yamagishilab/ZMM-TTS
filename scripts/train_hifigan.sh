#!/bin/bash
#SBATCH --job-name=HifiGAN
#SBATCH --out=Train_log/HifiGAN/train.log
#SBATCH --error=Train_log/HifiGAN/train.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python Vocoder_HifiGAN_Model/train.py --config Config/config_16k_mel.json
