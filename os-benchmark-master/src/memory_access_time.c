#include "bench.h"
#include "stdio.h"
#include "stdlib.h"

const int LOOP_TIME = 10;
const int TRY_TIME = 1000;

int benchmark_mem_with_offset(int size_offset, int step)
{
  int size = 1 << size_offset;
  int *mem = malloc(size * sizeof(int));
  int i, j;
  if (!mem) {
    fprintf(stderr, "alloc size 1 << %d failed\n", size_offset);
    return 0;
  }
  for (i = 0; i < size; i++) {
    mem[i] = (i + step) % size;
  }
  for (i = 0; i < LOOP_TIME; i++) {
    int p = mem[0];
    cpu_cycle before = rdtsc();
    for (j = 0; j < TRY_TIME; j++) {
      p = mem[p];
    }
    cpu_cycle after = rdtsc();
    double t = (after - before - get_timing_overhead())
                * 1.0 / TRY_TIME - get_loop_overhaead();
    fprintf(stdout, "offset %d overhead %f with step size %d "
            "and int size %lu\n", size_offset, t, step, sizeof(int *));
  }
  free(mem);
  return 1;
}

void benchmark_mem_acc()
{ 
  int i, j;
  get_timing_overhead();
  get_loop_overhaead();
  for (i = 0; i < 28; i++) {
    for (j = 0; j < 20; j++) {
      benchmark_mem_with_offset(i, 1 << j);
    }
  }
}
