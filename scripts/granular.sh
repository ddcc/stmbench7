#!/bin/bash

set -e

OUT="/home/ddchen/experiments/stmbench7-3/full_varlazy"
export STM="tinySTM"

function run () {
    make clean
    make
#   ORIGINAL=1 make
    mkdir -p $OUT/$CONFIG
    ./scripts/run-threads.sh ./scripts/abort.sh 3 $OUT/$CONFIG
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
