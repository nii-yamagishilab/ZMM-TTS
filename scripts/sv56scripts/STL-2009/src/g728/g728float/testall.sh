#!/bin/sh
TV=../testvector

echo "Encoder Test"
for i in 1 2 3 4 6
do
	echo Encoding Vector $i
	./g728 -little enc $TV/IN$i.BIN bits.out
	echo Comparing bit-streams
	cmp $TV/INCW$i.BIN bits.out
done

echo ""
echo "Decoder Test"
for i in 1 2 3 4 5 6
do
	echo Decoding Vector $i
	./g728 -little -nopostf dec $TV/CW$i.BIN speech.out
	echo Comparing output speech
	cmp $TV/OUTA$i.BIN speech.out
done

echo ""
echo "Postfilter Test"
echo Postfilter Test
./g728 -little dec $TV/CW4.BIN speech.out
echo Comparing output speech
cmp $TV/OUTB4.BIN speech.out

# clean up
rm -f speech.out bstream.out

