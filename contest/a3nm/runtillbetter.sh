#!/bin/bash

touch output$2
(./a.out "$1" <(sed 1d input) output$2 < final_round.in >&2) &
PID=$!
head -1 output$2 > head1$2
while true;
do
  inotifywait -e MODIFY output$2 >/dev/null 2>/dev/null
  head -1 output$2 > head1b$2
  if diff head1$2 head1b$2 > /dev/null
  then
    echo nop >&2
  else
    echo OK >&2
    break
  fi
done
kill $PID
cat output$2

