scp input $1:$2
ssh "$1"
cd "$2"
./runtillbetter.sh $3 $4

