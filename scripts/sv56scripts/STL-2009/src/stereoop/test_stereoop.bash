#!/bin/bash
# example use of stereoop operations 

dir=./stereo_proc
file=sample
fileL=$dir/$file.L.32k.1ch.smp
fileR=$dir/$file.R.32k.1ch.smp
fileLR=$dir/$file.LR.32k.2ch.smp 
fileLR_W=$dir/$file.LR.32k.2ch.wav 
fileL2=$dir/$file.L.split.32k.1ch.smp
fileR2=$dir/$file.R.split.32k.1ch.smp

fileL3=$dir/$file.L.left.32k.1ch.smp
fileR3=$dir/$file.R.right.32k.1ch.smp

fileMAXENVAL=$dir/$file.maxenval.32k.1ch.smp
fileMONO=$dir/$file.mono.32k.1ch.smp


lev=-36
fileL_scaled=$dir/$file.L.$lev.32k.1ch.smp
fileR_scaled=$dir/$file.R.$lev.32k.1ch.smp
fileLR_scaled=$dir/$file.LR.$lev.32k.2ch.smp 
fileLR_scaled_W=$dir/$file.LR.$lev.32k.2ch.wav 
fileLR_scaled_maxenval=$dir/$file.LR.$lev.maxenval.32k.1ch.smp 

quiet="-q"


./stereoop $quiet -interleave $fileL $fileR    $fileLR
./stereoop $quiet -split      $fileLR          $fileL2  $fileR2
cmp $fileL $fileL2
cmp $fileR $fileR2

./stereoop $quiet -left $fileLR          $fileL3  
cmp $fileL $fileL3

./stereoop $quiet -right $fileLR         $fileR3  
cmp $fileR $fileR3

./stereoop $quiet -maxenval $fileLR      $fileMAXENVAL  
./stereoop $quiet -mono $fileLR          $fileMONO       



#scale signal to $lev based on maxenval energy downmix analysis signal

#Estimate level using maxenval downmix
#STL=~/bin/stl2005
STL=../binUnix
tmp=/tmp/dummy1
p56log=/tmp/p56.log.txt
\rm $tmp $p56log
$STL/sv56demo -q -log $p56log -sf 32000 -lev $lev  $fileMAXENVAL $tmp  >/dev/null

Scale=`cat $p56log | grep "Norm factor" | awk '{print $6}'`
echo Scaling required=$Scale

echo scaling left  
$STL/scaldemo -q -gain $Scale $fileL  $fileL_scaled >/dev/null
echo scaling right 
$STL/scaldemo -q -gain $Scale $fileR  $fileR_scaled >/dev/null

#interleave signal for CuT input
./stereoop $quiet -interleave $fileL_scaled $fileR_scaled   $fileLR_scaled


# extra safety check of final level of the scaled stereo signal
./stereoop $quiet -maxenval $fileLR_scaled  $fileLR_scaled_maxenval
\rm $tmp $p56log
$STL/sv56demo -q -log $p56log -sf 32000 -lev $lev $fileLR_scaled_maxenval $tmp >/dev/null 
Scale=`cat $p56log | grep "Norm factor" | awk '{print $6}'`
echo Scaling required post-scaldemo operation to $lev =$Scale
ASL=`cat $p56log | grep " Active speech level" | awk '{print $5}'` 
echo Level of maximum energy analysis signal $ASL dBov

#look at the level of each channel
$STL/sv56demo -q -log $p56log -sf 32000 -lev $lev $fileL_scaled  $tmp  >/dev/null
ASL=`cat $p56log | grep " Active speech level" | awk '{print $5}'`
echo Level of Left channel $ASL dBov

$STL/sv56demo -q -log $p56log -sf 32000 -lev $lev $fileR_scaled  $tmp  >/dev/null
ASL=`cat $p56log | grep " Active speech level" | awk '{print $5}'`
echo Level of Right channel $ASL dBov


#optionally create wav file headers using AFSP tool-set
#AFSP=~/bin/afsp
#COPYAUDIO=$AFSP/CopyAudio.exe
#if [ -r $COPYUAUDIO ]
#then
# $COPYAUDIO  -P "integer16,,32000,,," $fileL_scaled $fileR_scaled  -F wave-noex -D "integer16" -s 32000 -S "FL FR" $fileLR_scaled_W >/dev/null
#
# $COPYAUDIO  -P "integer16,,32000,,," $fileL  $fileR  -F wave-noex -D "integer16" -s 32000 -S "FL FR" $fileLR_W  >/dev/null
#fi
 
# end of file test_stereoop.bash
