#!/usr/bin/python
import re
import sys

def enable_event(cat, evt):
    with open('/sys/kernel/debug/tracing/events/%s/%s/enable' % (cat, evt), 'w') as fp:
        fp.write('1\n')

def tracing_enable(one_or_zero):
    with open('/sys/kernel/debug/tracing/tracing_enabled', 'w') as fp:
        fp.write('%s\n' % one_or_zero)

def tracing_reset():
    with open('/sys/kernel/debug/tracing/trace', 'w') as fp:
        fp.write('\n')

class TracePipe:
    def __init__(self):
        self.fp = open('/sys/kernel/debug/tracing/trace_pipe', 'r')
    def readline(self):
        return self.fp.readline()

if __name__ == '__main__':
    enable_event('irq', 'irq_handler_entry')
    enable_event('irq', 'irq_handler_exit')
    enable_event('sched', 'sched_switch')
    tracing_enable(1)
    tracing_reset()
    pipe = TracePipe()
    parser = re.compile(r'\[(\d+)\].* (\d+)\.(\d+):\s+([^:]+):')
    print('[')
    for _ in range(5000):
        line = pipe.readline()
        sys.stderr.write(line)
        cpu, sec, usec, evt = parser.finditer(line).next().groups()
        ts = int(sec)*(10**6)+int(usec)
        print('''{"name": "%s", "cat": "", "ts":%d, "ph": "X", "dur": 1, "pid": 0, "tid": %d},''' % (evt, ts, int(cpu)))
    print('""]')
    tracing_enable(0)
