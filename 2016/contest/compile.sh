#!/bin/bash

g++ -std=c++14 main.cpp -Wall -O2
for i in *.in ;
do
    echo "Doing $i " ;
    ./a.out < $i > ${i%%in}out ;
done ;
