#!/bin/bash
#SBATCH --job-name=vec2mel
#SBATCH --out=Train_log/vec2mel/output/train.log
#SBATCH --error=Train_log/vec2mel/output/train.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python vec2mel/train.py --dataset MM6 --config MM6

