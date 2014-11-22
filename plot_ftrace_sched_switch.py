#!/usr/bin/python
import os
import sys
import re

class Ts:
    def __init__(self, s=None):
        self.usec = 0
        if s:
            sec, usec = s.split('.')
            self.usec = int(sec)*1000*1000 + int(usec)
    def __sub__(self, that):
        t = Ts()
        t.usec = self.usec - that.usec
        return t
    def __add__(self, that):
        t = Ts()
        t.usec = self.usec + that.usec
        return t
    def __repr__(self):
        return '%d' % self.usec
        
class SchedSwitchEvent:
    r = re.compile(r'\s*(.*-\d+)\s+\[(\d+)\] (....) (\d+\.\d+): (sched_switch): prev_comm=(.+) prev_pid=(\d+) prev_prio=(\d+) prev_state=(.+) ==> ' +
            'next_comm=(.*) next_pid=(\d+) next_prio=(\d+)')
    def __init__(self, line):
        parse = SchedSwitchEvent.r.search(line).groups()
        self.task_pid = parse[0]
        self.cpu = int(parse[1])
        self.flags = parse[2]
        self.timestamp = parse[3]
        self.event = parse[4]
        self.prev_comm = parse[5]
        self.prev_pid = int(parse[6])
        self.prev_prio = int(parse[7])
        self.prev_state = parse[8]
        self.next_comm = parse[9]
        self.next_pid = int(parse[10])
        self.next_prio = int(parse[11])
    def __repr__(self):
        return repr(self.__dict__)

if __name__ == '__main__':
    s = 'timesync-1135  [000] d... 37198.317469: sched_switch: prev_comm=timesync prev_pid=1135 prev_prio=120 prev_state=S ==> next_comm=test_affinity next_pid=7601 next_prio=120'
    proc_name = sys.argv[1]
    tid_start = {}
    print('[')
    offset = -1
    while True:
        line = sys.stdin.readline()
        if not line or line[-1] != '\n':
            break
        if not 'sched_switch:' in line:
            continue
        ev = SchedSwitchEvent(line)
        if offset < 0:
            offset = Ts(ev.timestamp)
        if ev.prev_comm == proc_name:
            if tid_start.get(ev.prev_pid):
                start, end = Ts(tid_start[ev.prev_pid]), Ts(ev.timestamp)
                print('{"name":"%d", "cat":"cpu %d", "ts":%s, "dur": %s, "pid":"%s", "tid": %d, "ph": "X"},'%
                    (ev.cpu, ev.cpu, start-offset, end-start, proc_name, ev.prev_pid))
                del tid_start[ev.prev_pid]
        if ev.next_comm == proc_name:
            tid_start[ev.next_pid] = ev.timestamp
    print('""]')
