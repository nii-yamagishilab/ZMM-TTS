# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================

sv56=scripts/sv56scripts/batch_normRMSE.sh
TEMP_de=delete_origin
source scripts/sv56scripts/env.sh
Dir='Dataset/MM6/wavs/'
#Dir='Dataset/MM6/test_wavs/'

echo "-------start---------"
${sv56} ${Dir} ${TEMP_de} 
echo "---done--------------"
