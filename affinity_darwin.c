#include <stdio.h>
#include <mach/thread_policy.h>
#include <mach/task_info.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/thread_policy.h>
#include <mach/thread_act.h>
#include <pthread.h>

#include "affinity.h"


int numphyscpus() {
  int numcpus;
  size_t sizeofint = sizeof(numcpus);
  if (-1 == sysctlbyname("hw.physicalcpu", &numcpus, &sizeofint, NULL, 0)) perror("sysctl");
  return numcpus;
}

int pthread_create_with_cpu_affinity(pthread_t *restrict thread, int cpu,
                                     const pthread_attr_t *restrict attr,
                                     void *(*start_routine)(void *), void *restrict arg) {
  thread_affinity_policy_data_t policy_data = { cpu };
  int rv = pthread_create_suspended_np(thread, attr, start_routine, arg);
  mach_port_t mach_thread = pthread_mach_thread_np(*thread);
  if (rv != 0) {
    return rv;
  }
  thread_policy_set((thread_t)thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy_data, 1);
  thread_resume(mach_thread);
  return 0;
}

