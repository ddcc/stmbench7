#!/bin/bash

#set -e

if [ $# -ne 1 ]; then
  echo "$0 <iterations>"
  exit
fi

export TM_STATISTICS=1

for i in 1 2 4 8 16 32 64; do
  for ((j=1; j<=${1}; j++)); do
    echo "Running $i threads at iteration $j"
    RET=1
    while [ $RET -ne 0 ]; do
      ./sb7_tt -n ${i} >> sb7.$i.log
      RET=$?
    done
  done
done
