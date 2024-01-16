#!/bin/bash
#SBATCH --job-name=prior
#SBATCH --out=prior.log
#SBATCH --error=prior.error
#SBATCH --time=70:00:00
#SBATCH --cpus-per-task=16

python prepare_data/compute_attention_prior.py

