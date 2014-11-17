#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "affinity.h"

#define UNUSED(x) x __attribute__((unused))

void* f(void* UNUSED(msg)) {
  long long l = 0;
  while (1) {
    l++;
  }
}

void usage(char** argv) {
  puts("A program that runs 2 threads, with optional CPU affinity");
  printf("usage: %s regular|affine|antiaffine\n", argv[0]);
  exit(1);
}

int main(int argc, char** argv) {
  pthread_t first, second;
  if (argc != 2) {
    usage(argv);
  }
  if (strcmp(argv[1], "regular") == 0) {
    puts("creating regular threads");
    pthread_create(&first, NULL, f, "first");
    pthread_create(&second, NULL, f, "second");
  } else if (strcmp(argv[1], "affine") == 0) {
    puts("creating threads with a different same CPU affinity");
    pthread_create_with_cpu_affinity(&first, 1, NULL, f, "first");
    pthread_create_with_cpu_affinity(&second, 2, NULL, f, "second");
  } else if (strcmp(argv[1], "antiaffine") == 0) {
    puts("creating threads with the same CPU affinity");
    pthread_create_with_cpu_affinity(&first, 1, NULL, f, "first");
    pthread_create_with_cpu_affinity(&second, 1, NULL, f, "second");
  } else {
    usage(argv);
    return 1;
  }
  pthread_join(first, NULL);
  return 0;
}
