We examine how much interrupts and context switch would interfere with a microbenchmark.

This is a trace caught from a physical 8-core linux system at rest

    $ uname -a
    Linux tlv-lpgh8 3.13.0-40-generic #69~precise1-Ubuntu SMP Fri Nov 14 10:29:31 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux

We record the `sched:sched_switch` and the `irq:irq_handler_{exit,enter}` events.

This is the [script we used](./trace_noise.py), it uses [ftrace](http://elinux.org/Ftrace).

This is the [trace](https://rawgit.com/elazarl/cpu_affinity/master/misc/trace.html). Each tid is actually the CPU ID.

You can see that there are plenty of gpps of up to 0.5 MS, even for very busy CPUs.
