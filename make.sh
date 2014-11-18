#!/bin/bash
OS=`uname -s|tr '[A-Z]' '[a-z]'`
CC=gcc
[ ! "$OS" = darwin ] && CFLAGS="-fsanitize=address"
$CC -ggdb3 -Wall -Wextra -pthread -o test_affinity affinity_$OS.c main.c
