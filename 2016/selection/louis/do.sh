#!/bin/bash

for i in ../*in ; do cat $i | ./a.out > $i.out ; (wc -l $i.out | cut -d " " -f1; cat $i.out) > $i.out2 ; done

zip res.zip main.cc do.sh
mv res.zip ..
