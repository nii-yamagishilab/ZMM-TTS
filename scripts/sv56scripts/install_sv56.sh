#!/bin/bash
set -e

sv56=https://github.com/openitu/STL/archive/refs/tags/v2009.tar.gz
sv56_dir=$PWD/STL-2009
mark=.done-sv56
home=$PWD
if [ ! -f $mark ]; then
  if [ -z "$(which sv56demo)" ]; then
    if [ ! -f $(basename $sv56) ]; then
      wget -q $sv56 || exit 1
    fi
    echo 'Unpacking sv56 source files'
    [ -d $sv56_dir ] && rm -r $sv56_dir
    tar -xf $(basename $sv56) || exit 1
    echo 'Building sv56'
    cd $sv56_dir/src/sv56
    make -f makefile.unx || exit 1
    if [ ! -e $sv56_dir/src/sv56/sv56demo ]; then
	echo 'fail to compile sv56'
	exit 1
    fi
  fi
  cd $home
  touch $mark
fi

echo "export PATH=$sv56_dir/src/sv56:\$PATH" >> env.sh
