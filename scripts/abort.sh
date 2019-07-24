#!/bin/bash

set -e

LABEL="[A-Za-z0-9() -_]+: [0-9]+"

if [ $# -lt 2 ]; then
  echo "$0 <iterations> <threads>"
  exit
fi

COUNTER=0
while [ $COUNTER -lt $1 ]; do
  LOG="abort${COUNTER}.csv"
  echo "${LOG}"

  rm -f *.log

  $(dirname $0)/run.sh $2

  INNER=0
  for i in *.log; do
      FILENAME=$(basename $i)
      FILENAME=${FILENAME%.*}

      if [ $INNER -eq 0 ]; then
        echo "Benchmark" > "${LOG}"
        grep -E "${LABEL}" $i | cut -d : -f 1 >> "${LOG}"
      fi

      grep -E "[tT]ime[a-z:= ]* |${LABEL}" ${i} | sed -r "s/[A-Za-z0-9 ()_-]*[:= ]+//" | (echo ${FILENAME} && cat) | paste -d, "${LOG}" - > "${LOG}.tmp"
      mv "${LOG}.tmp" "${LOG}"

      INNER=$((INNER+1))
  done

  COUNTER=$((COUNTER+1))
done
