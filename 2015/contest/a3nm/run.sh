#!/bin/bash

trap 'kill $(jobs -p)' EXIT

ssh "$1" pkill a.out
scp input $1:$2
ssh "$1" "cd $2; ./runtillbetter.sh $3 $4"

