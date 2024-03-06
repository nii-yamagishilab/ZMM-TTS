#!/bin/bash
#SBATCH --job-name=train_msmc_lj_16k_one_stage
#SBATCH --out=train_mu1.log
#SBATCH --error=trainj_mu1.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate msmc
fi

python train.py -c examples/ljspeech/configs/msmc_vq_gan_mgpu_onestage.yaml
