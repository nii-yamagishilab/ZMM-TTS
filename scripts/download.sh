# ==============================================================================
# Copyright (c) 2024, Yamagishi Laboratory, National Institute of Informatics
# Author: Gong Cheng (gongchengcheng@tju.edu.cn)
# All rights reserved.
# ==============================================================================

wget -P Dataset/origin_data/ https://dl.fbaipublicfiles.com/mls/mls_english.tar.gz
wget -P Dataset/origin_data/ https://dl.fbaipublicfiles.com/mls/mls_french.tar.gz
wget -P Dataset/origin_data/ https://dl.fbaipublicfiles.com/mls/mls_german.tar.gz
wget -P Dataset/origin_data/ https://dl.fbaipublicfiles.com/mls/mls_spanish.tar.gz
wget -P Dataset/origin_data/ https://dl.fbaipublicfiles.com/mls/mls_portuguese.tar.gz
tar -xvf Dataset/origin_data/mls_english.tar.gz -C Dataset/origin_data/
tar -xvf Dataset/origin_data/mls_french.tar.gz -C Dataset/origin_data/
tar -xvf Dataset/origin_data/mls_german.tar.gz -C Dataset/origin_data/
tar -xvf Dataset/origin_data/mls_spanish.tar.gz -C Dataset/origin_data/
tar -xvf Dataset/origin_data/mls_portuguese.tar.gz -C Dataset/origin_data/

