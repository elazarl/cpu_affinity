# Running on different CPUs

## Goal

I'm trying to make Mac OS X runs two threads on different CPUs.

To do that, I used the `thread_policy_set` API.

To verify that it indeed worked - I used a small dtrace script,
together with [Google's trace viewer](https://github.com/google/trace-viewer)

## dtrace

The dtrace script printed an event every time a thread went on and off to the CPU.
The essence was, save to a thread-local variable `on-cpu` event:

```dtrace
sched:::on-cpu /execname == $1/ { self->ts = timestamp; }
```

When the same thread went off CPU print an event

```dtrace
sched:::off-cpu /self->ts/ { printf("%d-%d, CPU:%d", self->ts, timestamp, cpu); }
```

## trace viewer

The dtrace script produces a valid json consumable by `trace2html`.

Here is an example [trace output](https://rawgit.com/elazarl/cpu_affinity/master/trace.html).

Process `9052` defined both threads to be in different affinity sets, while process
`9096` defined both to be in the same affinity set.

Initially it looked like it worked, when setting the two threads with the same affinity, you can see
that they're always scheduled in parallel, on different CPUs.

## Problem

However, even when you're setting both threads to the same affinity group, they still run on different
CPUs!

What went wrong?
