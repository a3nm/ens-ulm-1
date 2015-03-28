#!/bin/bash

trap './cleanup.sh && kill $(jobs -p)' EXIT

I="1"
cat targets | while read l; do
  I=$(($I+1))
  echo "spawn $l"
  (./run.sh $l > runout$I) &
done

inotifywait -e MODIFY runout* >/dev/null 2>/dev/null

pkill run.sh
ps aux | grep runtillbetter |awk '{print $2}' | xargs kill
ps aux | grep run |awk '{print $2}' | xargs kill

BEST=0
for a in runout*
do
  NEW=$(head -1 $a | cut -d ' ' -f2)
  if [ "x$NEW" = "x" ]
  then
    echo nop > /dev/null
  else
    if [ $NEW -gt $BEST ]
    then
      head -1 $a
      cp $a input
    fi
  fi
done

./cleanup.sh
pkill run.sh

