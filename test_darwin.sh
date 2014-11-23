#!/bin/bash

./make.sh
killall test_affinity
./test_affinity affine&
sleep 2
./test_affinity antiaffine&
sleep 2
sudo ./tracecpu.d test_affinity >test/darwin/trace.txt
trace2html test/darwin/trace.txt
killall test_affinity
open test/darwin/trace.html
