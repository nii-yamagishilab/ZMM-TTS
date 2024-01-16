#!/bin/bash
#SBATCH --job-name=woxptovec
#SBATCH --out=Train_log/txt2vec/output_xptovec_wo/train.log
#SBATCH --error=Train_log/txt2vec/output_xptovec_wo/train.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python txt2vec/train.py --dataset MM6 --config MM6_XphoneBERT_wo
