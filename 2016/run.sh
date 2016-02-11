#!/bin/bash

for a in *.in ; do echo $a; ./a.out < $a  | grep -E "( L | D )" > ${a%.in}.out; cat <(wc -l < ${a%.in}.out) ${a%.in}.out | sponge ${a%.in}.out; done
