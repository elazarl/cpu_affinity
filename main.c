#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "affinity.h"

#define UNUSED(x) x __attribute__((unused))

void* f(void* UNUSED(msg)) {
  printf("tid: %d\n", get_thread_id());
  long long l = 0;
  while (1) {
    l++;
  }
}

void usage(char** argv) {
  puts("A program that runs threads, with optional CPU affinity");
  printf("usage: %s regular|affine|antiaffine [nthreads]\n", argv[0]);
  exit(1);
}

int main(int argc, char** argv) {
  pthread_t thread;
  int n;
  int ncpu = numphyscpus();
  if (argc != 3 && argc != 2) {
    usage(argv);
    return 1;
  }
  n = numphyscpus();
  if (argc == 3) {
    n = strtol(argv[2], NULL, 10);
  }
  printf("pid: %d, running %d threads\n", getpid(), n);
  if (n == 0) {
    puts("Number of threads should be an integer");
    return 2;
  }
  if (strcmp(argv[1], "regular") == 0) {
    puts("creating regular threads");
    for (; n>0; n--) {
      pthread_create(&thread, NULL, f, NULL);
    }
  } else if (strcmp(argv[1], "affine") == 0) {
    puts("creating threads with a different same CPU affinity");
    for (; n>0; n--) {
      pthread_create_with_cpu_affinity(&thread, n%ncpu, NULL, f, "first");
    }
  } else if (strcmp(argv[1], "antiaffine") == 0) {
    puts("creating threads with the same CPU affinity");
    for (; n>0; n--) {
      pthread_create_with_cpu_affinity(&thread, 0, NULL, f, "first");
    }
  } else {
    usage(argv);
    return 1;
  }
  pthread_join(thread, NULL);
  return 0;
}
