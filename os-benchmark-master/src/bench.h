#ifndef BENCH_H
#define BENCH_H

#include <stdlib.h>
#include <string.h>

typedef unsigned long long int cpu_cycle;

#define true 1
#define false 0

#define IS_EQUAL(s1, s2) (strcmp(s1, s2) == 0)

#if defined(__i386__)
static inline cpu_cycle rdtsc(void) {
  cpu_cycle x;
  asm volatile (".byte 0x0f, 0x31" : "=A" (x));
  return x;
}
#elif defined(__x86_64__)
static inline cpu_cycle rdtsc(void) {
  unsigned hi, lo;
  asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
  return ((cpu_cycle) lo) | (((cpu_cycle) hi) << 32);
}
#elif defined(__powerpc__)
static inline cpu_cycle rdtsc(void) {
  cpu_cycle result=0;
  cpu_cycle upper, lower,tmp;
  asm volatile(
                "0:                  \n"
                "\tmftbu   %0           \n"
                "\tmftb    %1           \n"
                "\tmftbu   %2           \n"
                "\tcmpw    %2,%0        \n"
                "\tbne     0b         \n"
                : "=r"(upper),"=r"(lower),"=r"(tmp)
              );
  result = upper;
  result = result << 32;
  result = result | lower;

  return (result);
}
#endif

#define RUN_TEN_TIMES(p)\
    p;p;p;p;p;p;p;p;p;p;

#define _OUT_LOOP_MM(out, loop_count, include_loop, block) {         \
      int i;                                          \
      double _timing_overehad = get_timing_overhead();\
      double _loop_overhead = 0.0;                    \
      if (include_loop)                               \
        _loop_overhead = get_loop_overhaead();        \
      cpu_cycle before_loop = rdtsc();                \
      for (i = 0; i < loop_count; i++) {              \
        block;                                        \
      }                                               \
      cpu_cycle end_loop = rdtsc();                   \
      out = (end_loop - before_loop - _timing_overehad) / loop_count - _loop_overhead; \
    }

#define OUT_LOOP_MM(out, loop_count, block) _OUT_LOOP_MM(out, loop_count, true, block)

#define _IN_LOOP_MM(out, loop_count, max, include_timing, print_mid, block) {\
      int i;                                          \
      int real_loop = loop_count;                     \
      unsigned long long delta = 0;                   \
      double _t_overhead = 0.0;                       \
      if (include_timing)                             \
        _t_overhead = get_timing_overhead();      \
      for (i = 0; i < loop_count; i++) {              \
        cpu_cycle before_op = rdtsc();                \
        block;                                        \
        cpu_cycle end_op = rdtsc();                   \
        cpu_cycle delta_op = end_op - before_op;      \
        if ((delta_op < max) && (delta_op > 0)) {     \
          delta += delta_op;                          \
          if (print_mid && include_timing) {          \
            fprintf(stdout, "%llu %f\n", delta_op, delta_op - _t_overhead);\
          } else if (print_mid && !include_timing) {  \
            fprintf(stdout, "%llu\n", delta_op);      \
          }                                           \
        } else {                                      \
          real_loop--;                                \
        }                                             \
      }                                               \
      out = delta * 1.0 / real_loop - _t_overhead;    \
    }

#define IN_LOOP_MM(out, loop_count, max, block)  _IN_LOOP_MM(out, loop_count, max, true, true, block)

double get_timing_overhead();
double get_loop_overhaead();

// bench marks
void benchmark_time_overhead();
void benchmark_loop_overhead();
void benchmark_proc_call();
void benchmark_sys_call();
void benchmark_cr_proc();
void benchmark_cr_thread();
void benchmark_process_context_switch();
void benchmark_thread_context_switch();
void benchmark_mem_acc();
void benchmark_filecache();
void benchmark_fileread();
void benchmark_contention();
void benchmark_ram_bw();
void benchmark_page_fault();
#endif

