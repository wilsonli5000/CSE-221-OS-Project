#include <stdio.h>
#include <unistd.h>
#include "bench.h"
#include <sys/wait.h>

#define LOOP_TIMES 10000
void benchmark_cr_proc() {
  int i, pid, delta;
  cpu_cycle before_call, son_after_call,
	father_after_call, complete_call;
  for (i = 0; i < LOOP_TIMES; i++) {
    before_call = rdtsc();
    pid = fork();
    if (pid < 0) {
      fprintf(stderr, "fork error\n");
      exit(0);
    }
    else if (pid == 0) {
      son_after_call = rdtsc();
      delta = son_after_call - before_call;
      fprintf(stdout, "son delta = %d\n", delta);
      fflush(stdout);
      exit(0);
    }
    father_after_call = rdtsc();
    delta = father_after_call - before_call;
    fprintf(stdout, "father delta = %d\n", delta);
    fflush(stdout);
    wait(NULL);
    complete_call = rdtsc();
    delta = complete_call - before_call;
    fprintf(stdout, "complete delta = %d\n", delta);
    fflush(stdout);
  }
  fprintf(stderr, "create process overhead complete, check output \n");
}

