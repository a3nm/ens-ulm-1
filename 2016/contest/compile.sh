#!/bin/bash

EXEC=${1:-./a.out}

tar zcf source.tgz *.sh *.cpp *.cc
g++ -std=c++14 main.cpp -Wall -O2
for i in *.in ;
do
    echo "Doing $i " ;
    $EXEC < $i > ${i%%in}out ;
done ;
