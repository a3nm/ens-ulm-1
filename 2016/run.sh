#!/bin/bash

for a in *.in ; do echo $a; ./a.out < $a  | egrep -E "( L | D )" > ${a%.in}.out;
  cat <(wc -l < ${a%.in}.out) ${a%.in}.out > mytmp;
  cp mytmp ${a%.in}.out; done
