#!/bin/bash

./make.sh

killall test_affinity
./test_affinity affine&
PID=$!
sleep 1
sudo bash -c "echo $PID > /sys/kernel/debug/tracing/set_ftrace_pid"
sudo bash -c 'echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable'
sudo bash -c 'cat /sys/kernel/debug/tracing/trace_pipe' | head -n 1000 > test/linux/trace_affine.txt
sudo bash -c 'echo 0 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable'
./test/linux/plot_ftrace_sched_switch.py test_affinity <./test/linux/trace_affine.txt >./test/linux/trace_affine.json
trace2html ./test/linux/trace_affine.json

killall test_affinity
./test_affinity antiaffine&
PID=$!
sleep 1
sudo bash -c "echo $PID > /sys/kernel/debug/tracing/set_ftrace_pid"
sudo bash -c 'echo 1 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable'
sudo bash -c 'cat /sys/kernel/debug/tracing/trace_pipe' | head -n 1000 > test/linux/trace_antiaffine.txt
sudo bash -c 'echo 0 > /sys/kernel/debug/tracing/events/sched/sched_switch/enable'
./test/linux/plot_ftrace_sched_switch.py test_affinity <./test/linux/trace_antiaffine.txt >./test/linux/trace_antiaffine.json
trace2html ./test/linux/trace_antiaffine.json
