#!/bin/awk
# --------------------------------------------------------
# AWK script to produce an ASCII printout of a "bogus"
# G.192-compliant encoded frame. It will take frame 
# sizes from the command line and print the sync header, 
# frame length, and softbits, alternating 1's and 0's.
#
# NOTE: the ascii output can be converted to bynary using 
# the unsupported demo program ascii2bin.c
#
# Simao -- February 2001
# --------------------------------------------------------
{
  print "0x6B21"
  printf("0x%00x\n", $1)
  for (i=0;i<$1;i++)
     print i%2? "0x007F":"0x0081"
}
