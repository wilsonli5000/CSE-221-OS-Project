#!/bin/bash
bwlocal="./netResult/bw_local.txt"
bwremote="./netResult/bw_remote.txt"

#<<COMMENT1
if [ -f "$bwlocal" ]
then
	rm "$bwlocal"
fi

for i in {1..10}
do
	 ./client 127.0.0.1 >> "$bwlocal"
done
#COMMENT1

<<COMMENT2

if [ -f "$bwremote" ]
then
	rm "$bwremote"
fi

for i in {1..10}
do
	taskset -c 0x1 nice -20 ./client 192.168.1.2 >> "$bwremote"
done

COMMENT2
