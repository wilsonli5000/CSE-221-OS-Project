#!/bin/bash

result_dir="results"
RUN_FLAG="nice -20"

if [[ `uname` == "Linux" ]]
then
  RUN_FLAG="$RUN_FLAG taskset -c 0x1"
  echo $RUN_FLAG
fi

if ! [ -d $result_dir ]
then
  mkdir $result_dir
fi

for bench in `bin/benchmark help`
do
  if [[ $bench == Supported || $bench == benchmarks: ]]
  then
    continue
  fi
  echo "$RUN_FLAG bin/benchmark $bench > $result_dir/${bench}_result"
  $RUN_FLAG bin/benchmark $bench > $result_dir/${bench}_result
done
