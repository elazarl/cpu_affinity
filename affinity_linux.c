#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>

#include "affinity.h"


int numphyscpus() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}

struct affinity_trampoline {
  cpu_set_t cpuset;
  void* (*start_routine)(void*);
  void* arg;
};


static void* set_affinity_trampoline(void* arg) {
  struct affinity_trampoline* trampoline = arg;
  sched_setaffinity(0, sizeof(cpu_set_t), &trampoline->cpuset);
  void* trampoline_arg = trampoline->arg;
  void* (*start_routine)(void*) = trampoline->start_routine;
  free(trampoline);
  return start_routine(trampoline_arg);
}

int pthread_create_with_cpu_affinity(pthread_t *__restrict thread, int cpu,
                                     const pthread_attr_t *__restrict attr,
                                     void *(*start_routine)(void *), void *__restrict arg) {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu, &cpuset);

  struct affinity_trampoline* trampoline = malloc(sizeof(*trampoline));
  struct affinity_trampoline t = { cpuset, start_routine, arg };
  *trampoline = t;
    
  pthread_create(thread, attr, set_affinity_trampoline, trampoline);
  return 0;
}

