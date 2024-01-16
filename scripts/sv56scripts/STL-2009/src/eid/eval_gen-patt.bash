#!/bin/bash -v


#generate

preamble=500
start=`expr $preamble + 1`
n=1500
rate=0.03

\rm sta; 
./gen-patt -fer -g192 -start $start -n $n -rate $rate  FER.start$start.n$n.rate$rate.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  FER.start$start.n$n.rate$rate.g192  preamble
astrip  -sample -start $start   FER.start$start.n$n.rate$rate.g192  ferpart
ep-stats preamble
ep-stats ferpart

# now test with iteration

tol=0.0005
max=80

\rm sta; 
./gen-patt -fer -g192 -start $start -n $n -rate $rate -tol $tol  -max $max  FER.start$start.n$n.rate$rate.tol$tol.max$max.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  FER.start$start.n$n.rate$rate.tol$tol.max$max.g192  preamble
astrip  -sample -start $start   FER.start$start.n$n.rate$rate.tol$tol.max$max.g192  ferpart
ep-stats preamble
ep-stats ferpart


# 
# TAILSTAT , evaluates stst on tail part , not on preamble
# now test with tailstat option
#
\rm sta; 
./gen-patt  -fer -g192 -start $start -n $n -rate $rate -tailstat  FER.start$start.n$n.rate$rate.tailstat.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  FER.start$start.n$n.rate$rate.tailstat.g192  preamble
astrip  -sample -start $start   FER.start$start.n$n.rate$rate.tailstat.g192  ferpart
ep-stats preamble
ep-stats ferpart

# now test with iteration and tailstat


\rm sta; 
./gen-patt -fer -g192 -start $start -n $n -rate $rate -tol $tol  -max $max -tailstat  FER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  FER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  preamble
astrip  -sample -start $start   FER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  ferpart
ep-stats preamble
ep-stats ferpart


## additional tests 
# now test with BFER iteration and tailstat

\rm sta; 
./gen-patt -bfer -g192 -start $start -n $n -rate $rate -tol $tol  -max $max -tailstat  BFER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  BFER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  preamble
astrip  -sample -start $start   BFER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  ferpart
ep-stats preamble
ep-stats ferpart


# now test with BER iteration and tailstat


\rm sta; 
./gen-patt -ber -g192 -start $start -n $n -rate $rate -tol $tol  -max $max -tailstat  BER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192
#check output using epstats
astrip  -sample -start 1 -n $preamble  BER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  preamble
astrip  -sample -start $start   BER.start$start.n$n.rate$rate.tol$tol.max$max.tailstat.g192  ferpart
ep-stats preamble
ep-stats ferpart





#end 




 

