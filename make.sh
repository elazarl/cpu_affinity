#!/bin/bash
OS=`uname -s|tr '[A-Z]' '[a-z]'`
CC=clang
$CC -ggdb3 -fsanitize=address -Wall -Wextra -pthread -o test_affinity affinity_$OS.c main.c
