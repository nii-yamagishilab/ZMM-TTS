#!/bin/bash -v


#generate

preamble=80
start=`expr $preamble + 1`
n=240
rate=0.30

\rm sta; 
./gen-patt -fer -g192 -start $start -n $n -rate $rate -tailstat  FER.start$start.n$n.rate$rate.tailstat.g192
#check output using epstats

# no start option 
echo ./ep-stats FER.start$start.n$n.rate$rate.tailstat.g192 
./ep-stats FER.start$start.n$n.rate$rate.tailstat.g192 
# with start option
echo ./ep-stats -start $start FER.start$start.n$n.rate$rate.tailstat.g192 
./ep-stats -start $start FER.start$start.n$n.rate$rate.tailstat.g192 


#extract parts seprately
astrip  -sample -start 1 -n $preamble  FER.start$start.n$n.rate$rate.tailstat.g192  preamble
astrip  -sample -start $start   FER.start$start.n$n.rate$rate.tailstat.g192  ferpart

echo ./ep-stats preamble
./ep-stats preamble
#
echo ./ep-stats ferpart
./ep-stats ferpart



# now with teh other error formats byte . byte
./gen-patt -fer -byte  -start $start -n $n -rate $rate -tailstat  FER.start$start.n$n.rate$rate.tailstat.byte
#check output using epstats

# no start option 
echo ./ep-stats -fer -ep byte FER.start$start.n$n.rate$rate.tailstat.byte
./ep-stats -fer -ep byte FER.start$start.n$n.rate$rate.tailstat.byte
# with start option
echo ./ep-stats -fer -ep byte -start $start FER.start$start.n$n.rate$rate.tailstat.byte 
./ep-stats -fer -ep byte -start $start FER.start$start.n$n.rate$rate.tailstat.byte


# now with the other error formats bit
./gen-patt -fer -bit  -start $start -n $n -rate $rate -tailstat  FER.start$start.n$n.rate$rate.tailstat.bit
#check output using epstats

# no start option 
echo ./ep-stats -fer -ep bit FER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -fer -ep bit FER.start$start.n$n.rate$rate.tailstat.bit
# with start option
echo ./ep-stats -fer -ep bit -start $start FER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -fer -ep bit -start $start FER.start$start.n$n.rate$rate.tailstat.bit

# with illegal start option
start2=`expr $preamble + 2`

echo ./ep-stats -fer -ep bit -start $start2 FER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -fer -ep bit -start $start2 FER.start$start.n$n.rate$rate.tailstat.bit

#legal start at 1
echo ./ep-stats -fer -ep bit -start 1 FER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -fer -ep bit -start 1 FER.start$start.n$n.rate$rate.tailstat.bit



# now with the other error formats  ber 
./gen-patt -ber -bit  -start $start -n $n -rate $rate -tailstat  BER.start$start.n$n.rate$rate.tailstat.bit
#check output using epstats

# no start option 
echo ./ep-stats -ber -ep bit BER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -ber -ep bit BER.start$start.n$n.rate$rate.tailstat.bit
# with start option
echo ./ep-stats -ber -ep bit -start $start BER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -ber -ep bit -start $start BER.start$start.n$n.rate$rate.tailstat.bit

# with illegal start option
echo ./ep-stats -ber -ep bit -start $start2 BER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -ber -ep bit -start $start2 BER.start$start.n$n.rate$rate.tailstat.bit

#legal start at 1
echo ./ep-stats -ber -ep bit -start 1 BER.start$start.n$n.rate$rate.tailstat.bit 
./ep-stats -ber -ep bit -start 1  BER.start$start.n$n.rate$rate.tailstat.bit





#end





 

