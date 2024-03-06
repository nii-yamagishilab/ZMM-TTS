#!/bin/sh

PLCLEN=10
MASK=mask1
INPUT=../testvector/IN5.BIN

echo "Encoding Vector 5"
./g728fp -little enc $INPUT bits.out
echo "Decoding with packet loss concealment"
./g728fp -stats -little -plcsize $PLCLEN plc bits.out $MASK speech.out
