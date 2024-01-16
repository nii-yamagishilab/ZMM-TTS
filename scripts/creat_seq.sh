#!/bin/bash
#SBATCH --job-name=creat_seq
#SBATCH --out=seq.log
#SBATCH --error=seq.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1
conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
     . ${conda_setup}
     conda activate cssf
fi

python prepare_data/extract_text_seq_from_raw_text.py
