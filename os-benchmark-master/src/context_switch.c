#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "bench.h"
#include <sys/wait.h>

#define LOOP_COUNT 1000

void benchmark_process_context_switch() {
  int fd[4];
  int i;
  if (pipe(fd))
    goto pipe_create_error;
  if (pipe(&fd[2]))
    goto pipe_create_error;
  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Can't create new process");
    exit(1);
  }
  if (pid == 0) {
    close(fd[0]);
    close(fd[3]);
    for (i = 0; i < LOOP_COUNT; i++) {
      cpu_cycle before_switch = rdtsc();
      write(fd[1], &before_switch, sizeof(before_switch));
      read(fd[2], &before_switch, sizeof(before_switch));
      cpu_cycle after_switch = rdtsc();
      int delta = after_switch - before_switch;
      fprintf(stdout, "parent_switch_to_son %d\n", delta);
      fflush(stdout);
    }
    close(fd[1]);
    close(fd[2]);
    exit(0);
  }
  close(fd[1]);
  close(fd[2]);
  for (i = 0; i < LOOP_COUNT; i++) {
    cpu_cycle before_switch = 0;
    read(fd[0], &before_switch, sizeof(before_switch));
    cpu_cycle after_switch = rdtsc();
    int delta = after_switch - before_switch;
    fprintf(stdout, "son_switch_to_parent %d\n", delta);
    before_switch = rdtsc();
    write(fd[3], &before_switch, sizeof(before_switch));
    fflush(stdout);
  }
  close(fd[0]);
  close(fd[3]);
  wait(NULL);
  return;
pipe_create_error:
  fprintf(stderr, "Can't create pipe");
  exit(1);
}

int fd[4];

void* thread_proc() {
  int i;
  for (i = 0; i < LOOP_COUNT; i++) {
    cpu_cycle before_switch = rdtsc();
    write(fd[1], &before_switch, sizeof(before_switch));
    read(fd[2], &before_switch, sizeof(before_switch));
    cpu_cycle after_switch = rdtsc();
    int delta = after_switch - before_switch;
    fprintf(stdout, "parent_switch_to_son %d\n", delta);
    fflush(stdout);
  }
  return NULL;
}

void benchmark_thread_context_switch() {
  pthread_t thread;
  if (pipe(fd))
    goto pipe_create_error;
  if (pipe(&fd[2]))
    goto pipe_create_error;

  pthread_create(&thread, NULL, thread_proc, NULL);

  int i;
  for (i = 0; i < LOOP_COUNT; i++) {
    cpu_cycle before_switch = 0;
    read(fd[0], &before_switch, sizeof(before_switch));
    cpu_cycle after_switch = rdtsc();
    int delta = after_switch - before_switch;
    fprintf(stdout, "son_switch_to_parent %d\n", delta);
    before_switch = rdtsc();
    write(fd[3], &before_switch, sizeof(before_switch));
    fflush(stdout);
  }
  close(fd[0]);
  close(fd[1]);
  close(fd[2]);
  close(fd[3]);
  pthread_join(thread, NULL);
  return;
pipe_create_error:
  fprintf(stderr, "Can't create pipe");
  exit(1);
}
