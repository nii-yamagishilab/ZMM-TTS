#!/bin/bash
#SBATCH --job-name=xptovec
#SBATCH --out=Train_log/txt2vec/output_xptovec/train.log
#SBATCH --error=Train_log/txt2vec/output_xptovec/train.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100_80g:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python txte2vec/train.py --dataset MM6 --config MM6_XphoneBERT


