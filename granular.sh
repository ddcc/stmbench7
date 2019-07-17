#!/bin/bash

set -e

OUT="/home/ddchen/experiments/stmbench7-3/full_varlazy"
export STM="tinySTM"

function run () {
    make clean
    make
    ./run-threads.sh 4
    mkdir -p $OUT/$CONFIG
    mv *.log $OUT/$CONFIG
}

CONFIG="all"
git checkout src/operations/operations.cc
run

CONFIG="t3"
cp src/operations/operations_$CONFIG.cc src/operations/operations.cc
run

CONFIG="op11"
cp src/operations/operations_$CONFIG.cc src/operations/operations.cc
run
