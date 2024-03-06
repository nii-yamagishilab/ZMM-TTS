#!/bin/bash
#SBATCH --job-name=train_msmc_lj_16k_codeemb_wav_vq_wo_mel
#SBATCH --out=train.log
#SBATCH --error=trainj.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate msmc
fi

python train.py -c examples/ljspeech/configs/msmc_vq_gan_16k_unifed_code_wav_vq_womel.yaml
