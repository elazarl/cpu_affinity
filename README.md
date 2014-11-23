# Running on different CPUs

## Goal

I'm trying to make Mac OS X runs two threads on different CPUs.

To do that, I used the `thread_policy_set` API.

To verify that it indeed worked - I used a small dtrace script,
together with [Google's trace viewer](https://github.com/google/trace-viewer)

## Mac OS X

### dtrace

The dtrace script printed an event every time a thread went on and off to the CPU.
The essence was, save to a thread-local variable `on-cpu` event:

```dtrace
sched:::on-cpu /execname == $1/ { self->ts = timestamp; }
```

When the same thread went off CPU print an event

```dtrace
sched:::off-cpu /self->ts/ { printf("%d-%d, CPU:%d", self->ts, timestamp, cpu); }
```

### trace viewer

The dtrace script produces a valid json consumable by `trace2html`.

Here is an example [trace output](https://rawgit.com/elazarl/cpu_affinity/master/test/darwin/trace.html).

Process `89397` defined both threads to be in different affinity sets, while process
`89442` defined both to be in the same affinity set.

Initially it looked like it worked, when setting the two threads with the same affinity, you can see
that they're always scheduled in parallel, on different CPUs.


## Linux

### ftrace

ftrace event, sched_switch should trace threads execution. We'll use the `./test/linux/plot_ftrace_sched_switch.py`
script to convert them into google traceviewer files.

### Results

To obtain graphs, make sure you have `trace2html` in your path, and run

    ./test_linux.sh

Here is an example [trace output with affinity per thread](https://rawgit.com/elazarl/cpu_affinity/master/test/linux/trace_affine.html).
another [trace output without affinity per thread](https://rawgit.com/elazarl/cpu_affinity/master/test/linux/trace_antiaffine.html).

## Problem

Linux looks fine.

However, in Mac OS X, even when you're setting both threads to the same affinity group, they still run on different
CPUs!

What went wrong?
