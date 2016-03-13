#include <stdio.h>
#include <stdlib.h>
#include "bench.h"

#define LOOP_TIMES 10000

#define PC(i) proc_call_##i
static void PC(0)() {}
static void PC(1)(int p0) {}
static void PC(2)(int p0, int p1) {}
static void PC(3)(int p0, int p1, int p2) {}
static void PC(4)(int p0, int p1, int p2, int p3) {}
static void PC(5)(int p0, int p1, int p2, int p3, int p4) {}
static void PC(6)(int p0, int p1, int p2, int p3, int p4, int p5) {}
static void PC(7)(int p0, int p1, int p2, int p3, int p4, int p5, int p6) {}

const int MAX_PROC_CALL_CYCLE = 1000;
double proc_call_overhead(int arg_count) {
  double overhead;
  switch(arg_count) {
    default:
    case 0:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(0)()));
      break;
    case 1:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(1)(1)));
      break;
    case 2:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(2)(1, 2)));
      break;
    case 3:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(3)(1, 2, 3)));
      break;
    case 4:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(4)(1, 2, 3, 4)));
      break;
    case 5:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(5)(1, 2, 3, 4, 5)));
      break;
    case 6:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(6)(1, 2, 3, 4, 5, 6)));
      break;
    case 7:
      IN_LOOP_MM(overhead, LOOP_TIMES, MAX_PROC_CALL_CYCLE, RUN_TEN_TIMES(PC(7)(1, 2, 3, 4, 5, 6, 7)));
      break;
  }
  return overhead;
}

void benchmark_proc_call() {
  int i;
  for (i = 0; i <= 7; i++) {
    fprintf(stdout, "--------------------------\n");
    proc_call_overhead(i);
  }
}
