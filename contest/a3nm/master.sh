#!/bin/bash

I="1"
cat targets | while read l; do
  I=$(($I+1))
  echo "spawn $l"
  (./run.sh $l > runout$I) &
done

inotifywait -e MODIFY runout* >/dev/null 2>/dev/null

pkill run.sh

head -1 runout*

