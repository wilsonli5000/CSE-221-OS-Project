#include "bench.h"
#include <stdio.h>

typedef void (*benchmark_func)();

typedef struct _benchmark {
  char name[32];
  benchmark_func func;
} benchmark_t;

benchmark_t benchmarks[] = {
  {"timing_overhead", benchmark_time_overhead},
  {"loop_overhead", benchmark_loop_overhead},
  {"proc_call_overhead", benchmark_proc_call},
  {"sys_call_overhead", benchmark_sys_call},
  {"cr_proc_overhead", benchmark_cr_proc},
  {"cr_thread_overhead", benchmark_cr_thread},
  {"process_context_switch", benchmark_process_context_switch},
  {"thread_context_switch", benchmark_thread_context_switch},
  {"mem_acc", benchmark_mem_acc},
  {"filecache", benchmark_filecache},
  {"fileread", benchmark_fileread},
  {"filecontention", benchmark_contention},
  {"ram_bandwidth", benchmark_ram_bw},
  {"page_fault", benchmark_page_fault},
  {"", 0},
};

static inline void show_help() {
  fprintf(stdout, "Supported benchmarks:\n");
  benchmark_t *bm = benchmarks;
  while (bm->func != 0) {
    fprintf(stdout, "\t%s\n", bm->name);
    bm++;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Too few arguments. "
            "please run \"benchmark help\" for help.\n");
    exit(1);
  }
  if (IS_EQUAL(argv[1], "help")) {
    show_help();
    exit(0);
  }
  benchmark_t *bm = benchmarks;
  while (bm->func != 0) {
    if (IS_EQUAL(argv[1], bm->name)) {
      rdtsc();
      bm->func();
      exit(0);
    }
    bm++;
  }
  fprintf(stderr, "No such benchmark. "
          "please run \"benchmark help\" for help.\n");
  exit(1);
  return 0;
}
