#!/bin/sh
TV=../testvector

echo "Encoder Test"
for i in 1 2 3 4 5 6
do
	echo Encoding test vector $i
	./g728fp -little enc $TV/IN$i.BIN bits.out
	echo Comparing bit-streams
	cmp $TV/INCW${i}G.BIN bits.out
done

echo ""
echo "Decoder Test"
for i in 1 2 3 4 5 6
do
	echo Decoding test vector $i
	./g728fp -little -nopostf dec $TV/CW$i.BIN speech.out
	echo Comparing output speech
	cmp $TV/OUTA${i}G.BIN speech.out
done

echo ""
echo "Postfilter Test"
./g728fp -little dec $TV/CW4.BIN speech.out
echo Comparing output speech
cmp $TV/OUTB4G.BIN speech.out

rm -f speech.out bits.out
