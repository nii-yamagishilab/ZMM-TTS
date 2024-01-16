#!/bin/sh

# ==============================================================================
# Copyright (c) 2021, Yamagishi Laboratory, National Institute of Informatics
# Author: Xin Wang
# All rights reserved.
# ==============================================================================

### usage:
#   sh batch_normRMSE.sh PATH_TO_DIRECTORY_OF_WAVEFORM
# normalized waveforms will be in the input directory
# 
# note: 
# 1. make sure sox and sv56demo is in your path
# 2. make sure that SCRIPT_DIR points to the directory that contains batch_normRMSE.sh
###

# level of amplitude normalization, 26 by default
LEV=26

# input directory
DATA_DIR=$1

# argument, which can be delete_origin
TEMP=$2


SCRIPT_DIR=`realpath $0 | xargs -I{} dirname {} `

cd ${DATA_DIR}
for file_name in `ls ./ | grep wav`
do
    bash ${SCRIPT_DIR}/sub_normRMSE.sh ${file_name} ${SCRIPT_DIR} ${TEMP}
done

exit
