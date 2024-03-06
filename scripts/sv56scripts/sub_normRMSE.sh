#!/bin/bash

# ==============================================================================
# Copyright (c) 2021, Yamagishi Laboratory, National Institute of Informatics
# Author: Xin Wang
# All rights reserved.
# ==============================================================================

LEV=26

file_name=$1
SCRIPT_DIR=$2
TEMP=$3

if [ -e ${file_name} ];
then
    # basename
    basename=`basename ${file_name} .wav`
    # input file name
    INPUT=${file_name}
    # final output file name
    OUTPUT=${basename}_norm.wav
    # raw data name
    RAWORIG=${OUTPUT}.raw
    # normed raw data name
    RAWNORM=${OUTPUT}.raw.norm
    # 16bit wav
    BITS16=${OUTPUT}.16bit.wav

    SOX=sox
    SV56=sv56demo
    SCALE=${SCRIPT_DIR}/03_scale.py
    
    if ! type "${SOX}" &> /dev/null; then
	echo "${SOX} not in path"
	exit
    fi
    
    SAMP=`${SOX} --i -r ${INPUT}`
    BITS=`${SOX} --i -b ${INPUT}`

    # make sure input is 16bits int
    if [ ${BITS} -ne 16 ];
    then
	python ${SCALE} ${INPUT} ${INPUT}_tmp.wav
	${SOX} ${INPUT}_tmp.wav -b 16 ${BITS16}
	${SOX} ${BITS16} ${RAWORIG}
	rm ${BITS16}
	rm ${INPUT}_tmp.wav
    else
	${SOX} ${INPUT} ${RAWORIG}
    fi

    if ! type "${SV56}" &> /dev/null;
    then
	echo "convert but not normed ${INPUT} because ${SV56} is not in path"
	# convert but not normed
	${SOX} -t raw -b 16 -e signed -c 1 -r ${SAMP} ${RAWORIG} ${OUTPUT}
	rm ${RAWORIG}
    else
	#echo "convert and normed ${INPUT}"
	# norm the waveform
	${SV56} -q -sf ${SAMP} -lev -${LEV} ${RAWORIG} ${RAWNORM} > log_sv56 2>log_sv56

	# convert
	${SOX} -t raw -b 16 -e signed -c 1 -r ${SAMP} ${RAWNORM} ${OUTPUT}

	rm ${RAWNORM}
	rm ${RAWORIG}
    fi

    if [ "${TEMP}" = "delete_origin" ];
    then
	#echo " normalized waveforms saved to ${INPUT}"
	mv ${OUTPUT} ${INPUT}
    #else
	#echo " normalized waveforms saved to ${OUTPUT}"
    fi
else
    echo "not found ${file_name}"
fi

