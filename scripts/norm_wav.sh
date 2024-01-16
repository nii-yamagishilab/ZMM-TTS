#!/bin/bash
#SBATCH --job-name=norm_wav
#SBATCH --out=norm_wav.log
#SBATCH --error=norm_wav.error
#SBATCH --time=70:00:00
#SBATCH --cpus-per-task=16


sv56=scripts/sv56scripts/batch_normRMSE.sh
TEMP_de=delete_origin
source scripts/sv56scripts/env.sh
#Dir='Dataset/MM6/wavs/'
Dir='Dataset/MM6/test_wavs/'

echo "-------start---------"
${sv56} ${Dir} ${TEMP_de} 
echo "---done--------------"
