#!/bin/bash
#SBATCH --job-name=vec2wav
#SBATCH --out=Train_log/vec2wav/train.log
#SBATCH --error=Train_log/vec2wav/train.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100_80g:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python vec2wav/train.py -c Config/vec2wav/vec2wav.yaml
