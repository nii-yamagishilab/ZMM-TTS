#!/bin/bash
#SBATCH --job-name=test
#SBATCH --out=test.log
#SBATCH --error=test.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate cssf
fi

python3 synthesize.py --text "like his great predecessor old patch, he never went to a bank himself, nor did any of his accomplices." --restore_step 100000 --mode single --dataset LJSpeech 

