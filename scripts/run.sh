#!/bin/bash

set -e

export TM_STATISTICS=1

RET=1
while [ $RET -ne 0 ]; do
  ./sb7_tt -n $1 > sb7.log 2>&1
  RET=$?
done
