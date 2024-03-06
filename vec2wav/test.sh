#!/bin/bash
#SBATCH --job-name=train_msmc_lj_16k
#SBATCH --out=test.log
#SBATCH --error=testj.error
#SBATCH --time=70:00:00
#SBATCH --gres=gpu:tesla_a100:1

conda_setup="/home/others/v-cheng-gong/anaconda3/etc/profile.d/conda.sh"
if [[ -f "${conda_setup}" ]]; then
   . ${conda_setup}
   conda activate msmc
fi

python infer.py -c examples/ljspeech/configs/msmc_vq_gan_mgpu_onestage.yaml -m examples/ljspeech/checkpoints_mgpu/msmc_vq_gan/model_465000 -t examples/ljspeech/filelists/test_pred_code.yaml -o analysis_synthesis_hope
