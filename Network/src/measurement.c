#include <stdio.h>
#include <stdlib.h>
#include "bench.h"

#define LOOP_OVERHEAD_TAG "CSE211_LOOP_OVERHEAD"
#define TIMEING_OVERHEAD_TAG "CSE211_TIMING_OVERHEAD"
#define LOOP_TIMES 10000

static inline void setenv_double(char* tag, double d) {
  char env[32];
  sprintf(env, "%f", d);
  setenv(tag, env, true);
}

double get_timing_overhead() {
  if (getenv(TIMEING_OVERHEAD_TAG)) {
    double overhead = atof(getenv(TIMEING_OVERHEAD_TAG));
    return overhead;
  }

  double timing_overhead = 0.0;
  _IN_LOOP_MM(timing_overhead, LOOP_TIMES, 1000, false, false, );
  setenv_double(TIMEING_OVERHEAD_TAG, timing_overhead);
  return timing_overhead;
}

double get_loop_overhaead() {
  if (getenv(LOOP_OVERHEAD_TAG)) {
    double overhead = atof(getenv(LOOP_OVERHEAD_TAG));
    return overhead;
  }
  double loop_overhead;
  _OUT_LOOP_MM(loop_overhead, LOOP_TIMES, false, );
  setenv_double(LOOP_OVERHEAD_TAG, loop_overhead);
  return loop_overhead;
}

void benchmark_time_overhead() {
  double timing_overhead = 0.0;
  _IN_LOOP_MM(timing_overhead, LOOP_TIMES, 1000, false, true, );
  fprintf(stderr, "timing overhead: %f\n", timing_overhead);
}

void benchmark_loop_overhead() {
  int i;
  for (i = 0; i < LOOP_TIMES; i++) {
    double loop_overhead = 0.0;
    _OUT_LOOP_MM(loop_overhead, 1000, false, );
    fprintf(stdout, "%f\n", loop_overhead);
  }
}
