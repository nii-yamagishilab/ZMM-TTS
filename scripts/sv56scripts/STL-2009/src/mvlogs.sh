#!/bin/sh
for f in */*.log is54/bin/*.log rpeltp/ts/*.log
do 
 if test -d ${f%/*}/logs 
 then
  echo mv -f $f ${f%/*}/logs
 else
  echo Missing for this: ${f%/*}
 fi
done
