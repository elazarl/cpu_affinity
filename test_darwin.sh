#!/bin/bash

./make.sh
killall test_affinity
./test_affinity affine&
sleep 2
./test_affinity antiaffine&
sleep 2
sudo ./tracecpu.d test_affinity >trace.txt
trace2html trace.txt
killall test_affinity
open trace.html
