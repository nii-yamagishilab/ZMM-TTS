#!/bin/sh

while :
do
 case $1 in
 -if)
    order=`endian`
    if test "$2" = "$order" -o "$2" = "any"
    then
      shift ; shift 
    else
      shift ; shift
      echo System is $order-endian - not byteswapping: $*
      exit
    fi
    ;;
 -over)
    # Ignore the option since it already overwrites!
    shift ;; 
 *) 
    break 
    ;;
 esac
done

for f in $* ; do sb -over $f; done


##!/bin/sh
#for f in $* ; do sb -over $f; done
