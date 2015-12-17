#!/bin/sh

for d in 10 20 50 100 200;
do 
D2=`expr $d / 2`
echo "$D2 - $d"
nice time bin/gzkfast -s 1000 -e 5000 -t 3 -a -2.6 -l 50 -h 50000 -n $D2 -f $d -d 500 -r 500 -b 1e-12 -q 1e-6 -p "./data/proton$d" -2 "./data/secondaryproton$d" -v "./data/neutrino$d"  -g "./data/photon$d" -c "./data/cmb$d"

done
