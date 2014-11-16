#!/usr/bin/env dtrace -s

#pragma D option quiet

dtrace:::BEGIN
{
	printf("[");
}

dtrace:::END
{
	printf("\"\"]");
}

sched:::on-cpu
/execname == $$1/
{
	self->ts = timestamp;
}

sched:::off-cpu
/self->ts/
{
	printf("{\"name\":\"%d\", \"cat\":\"cpu %d\", \"ts\":%d, \"dur\": %d, \"pid\":%d, \"tid\": %d, \"ph\": \"X\"},\n", cpu, cpu, self->ts, timestamp-self->ts, pid, tid);
}

profile:::tick-1000ms {
	exit(0);
}
