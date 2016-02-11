 for i in ../*in ; do cat $i | ./a.out > $i.out ; (wc -l $i.out ; cat $i.out) > $i.out2 ; done

