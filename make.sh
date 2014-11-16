#!/bin/bash
OS=`uname -s|tr '[A-Z]' '[a-z]'`
gcc -o test_affinity affinity_$OS.c main.c
