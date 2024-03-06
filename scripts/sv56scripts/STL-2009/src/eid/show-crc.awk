#!/bin/awk
#----------------------------------------------------
# AWK script to print the CRC info from the test
# log printout produced by the EID makefiles.
#----------------------------------------------------
/gawk/{getline;while ($0 ~ /^[-0-9]/) {print;getline}; print ""}
