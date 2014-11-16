#ifndef __H_AFFINITY_
#define __H_AFFINITY_

#include <pthread.h>

int numphyscpus();
int pthread_create_with_cpu_affinity(pthread_t *restrict thread, int cpu,
  const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg);

#endif // __H_AFFINITY_
