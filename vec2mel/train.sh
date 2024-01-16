#!/bin/bash
#SBATCH --job-name=train_codeembtom_16k
#SBATCH --out=train.log
#SBATCH --error=trainj.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python3 train.py --dataset LJSpeech
