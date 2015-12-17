#!/bin/sh

nice -n +19 time bin/gzkfast -s 1 -e 500 -t 2 -a -2.6 -l 1 -h 1e4 -n 10 -f 100 -dx 1000 -r 1000 -b 1e-12 -q 1e-4 -p "./data/proton" -2 "./data/proton2" -v "./data/nu"  -g "./data/photon" -c "./data/cmb"
