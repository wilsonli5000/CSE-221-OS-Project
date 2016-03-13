#define _GNU_SOURCE
#include "bench.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// for test
#ifndef O_DIRECT
#define O_DIRECT 0
#endif

#define FSMAXSIZE ((unsigned int)(1 << 30) * 2)
#define GAPSIZE   (1 << 27)
#define MAXFSIZE  (1 << 29)
#define MINFSIZE  (1 << 20)
#define CONTSIZE  (1 << 25)
#define CONTPROC  (1 << 7)
#define BUFSIZE   2048

#define FILEBUFSIZE (128 * 1024)

#define LOOP_NUM 2

void benchmark_filecache()
{
  char filename[] = "./fileCacheBench.XXXXXX";
  unsigned int target_size = 0;
  unsigned int cursize = 0;
  int fd;
  char buf[BUFSIZE];
  int res;
  
  memset(buf, '0', BUFSIZE);
  if ((fd = mkstemp(filename)) == -1) {
    fprintf(stderr, "Create file failed");
    goto error_fc;
  }
  fprintf(stderr, "Temporary file name is %s\n", filename);
  for (target_size= (GAPSIZE << 3) + GAPSIZE * 5; target_size < FSMAXSIZE; target_size += GAPSIZE) {
    while (cursize < target_size) {
      if ((res = write(fd, buf, ((target_size - cursize) % BUFSIZE) + 1)) == -1)
        goto error_fc;
      cursize += res;
    }
    system("sync");
    system("sudo /sbin/sysctl vm.drop_caches=3");
    int i;
    cpu_cycle before_measure = rdtsc();
    for (i = 0; i < LOOP_NUM; i++) {

      if (lseek(fd, 0, SEEK_SET) == -1) goto error_fc;

      do {
        if ((res = read(fd, buf, BUFSIZE)) == -1) goto error_fc;
      } while(res);
    }
    cpu_cycle after_measure = rdtsc();
    double avg = (after_measure - before_measure) / LOOP_NUM;
    fprintf(stdout, "%dMB %f\n", cursize >> 20, avg);
  }
  goto exit_fc;
  error_fc:
    perror("something wrong");
  exit_fc:
    if (unlink(filename) == -1) {
      perror("unlink error");
    }
}

void benchmark_fileread()
{
  //char filename[] = "./fileReadBench.XXXXXX";
  char filename[] = "/home/cse221/nfs-test/fileReadBench.XXXXXX";
  unsigned long long target_size = 0;
  unsigned long long cursize = 0;
  int fd;
  char *buf;
  int res;
  int readed;

  srandom(time(NULL) + getpid());
  #define BLKSIZE 4096
  //memalign(BLKSIZE, BLKSIZE);
  posix_memalign((void **)&buf, BLKSIZE, FILEBUFSIZE);
  memset(buf, '0', FILEBUFSIZE);
  if ((fd = mkstemp(filename)) == -1) goto error_fr;
  fprintf(stderr, "Temporary file name is %s \n", filename);
  long long int MAXSIZE = 1;
  MAXSIZE <<= 31;
  close(fd);
  for (target_size = MINFSIZE; target_size < MAXSIZE; target_size = (target_size << 1)) {
    fd = open(filename, O_APPEND|O_RDWR);
    while (cursize < target_size) {
      if ((res = write(fd, buf, ((target_size - cursize) % FILEBUFSIZE) + 1)) ==  -1)
        goto error_fr;
      cursize += res;
      //fprintf(stderr, "Wrote %d bytes of %d bytes\n", cursize, target_size);
    }
    close(fd);
    system("sync");
    system("sudo /sbin/sysctl vm.drop_caches=3");
    fd = open(filename, O_SYNC|O_DIRECT|O_RDWR);
    if (fd < 0) goto error_fr;

    int i;
    cpu_cycle before_measure;
    cpu_cycle after_measure;
    double avg;
    before_measure = rdtsc();
    for (i = 0; i < LOOP_NUM; i++) {
      readed = 0;
      do {
        res = random() % (cursize - FILEBUFSIZE);
        res = (res - (res % FILEBUFSIZE));
        if (lseek(fd, res, SEEK_SET) == -1){
          goto error_fr;
        }
        // fprintf(stderr, "%d readed %d remain\n", readed, cursize - readed);
        if ((res = read(fd, buf, FILEBUFSIZE)) == -1 || res == 0){
          fprintf(stderr, "Read error\n");   
          goto error_fr;
        }
        readed += res;
        //fprintf(stderr, "Readed %d bytes of %d bytes\r", readed, cursize);
      } while(readed < cursize);
    }
    after_measure = rdtsc();
    avg = (after_measure - before_measure) / LOOP_NUM;
    fprintf(stdout, "Random read of %lluMB file = %f cycles\n", cursize >> 20, avg);

    before_measure = rdtsc();
    for (i = 0; i < LOOP_NUM; i++) {
      if (lseek(fd, 0, SEEK_SET) == -1) goto error_fr;
      do {
        if ((res = read(fd, buf, FILEBUFSIZE)) == -1) goto error_fr;
      } while(res);
    }
    after_measure = rdtsc();
    avg = (after_measure - before_measure) / LOOP_NUM;
    fprintf(stdout, "SEQ read of %lluMB file = %f cycles\n", cursize >> 20, avg);
    close(fd);
  }
  goto exit_fr;
  error_fr:
    perror("something wrong");
  exit_fr:
    if (unlink(filename) == -1) {
      perror("unlink error");
    }
}

void benchmark_contention()
{
  char filename[CONTPROC][80]; 
  char buf[FILEBUFSIZE];
  int fd;
  int res;
  unsigned int pi, j, size;
  memset(buf, '0', FILEBUFSIZE);
  for (pi = 0; pi < CONTPROC; pi++) {
    sprintf(filename[pi], "./contention/fileContentionBench%d.XXXXXX", pi);
    if ((fd = mkstemp(filename[pi])) == -1) goto error_frc;
    for (size = 0; size < CONTSIZE; size+= res) {
      if ((res = write(fd, buf, FILEBUFSIZE)) == -1) goto error_frc;
    }
    fprintf(stderr, "Temporary file %s created.\n", filename[pi]);
    close(fd);
  }
  system("sync");
  system("sudo /sbin/sysctl vm.drop_caches=3");
  int proc;
  cpu_cycle before_measure, after_measure;
  double avg;
  int i;
  for (proc = 1; proc <= CONTPROC; proc = proc << 1) {
    fprintf(stderr, "%d processes ... \n", proc);
    for (j = 0; j < proc; j++) {
      res = fork();
      if (res < 0) goto error_frc;
      if (res > 0) continue;
      
      fd = open(filename[j], O_RDONLY);
      if (fd < 0) goto error_chd;
      before_measure = rdtsc();
      for (i = 0; i < LOOP_NUM; i++) {
        if (lseek(fd, 0, SEEK_SET) == -1) goto error_chd;
        do {
          if ((res = read(fd, buf, FILEBUFSIZE)) == -1) goto error_chd;
        } while(res);
      }
      after_measure = rdtsc();
      avg = (after_measure - before_measure) / LOOP_NUM;
      fprintf(stdout, "Sequential read of %dMB file = %f cycles\n", CONTSIZE >> 20, avg);
      close(fd);
      exit(0);
      error_chd:
        perror("child error");
        exit(1);
    }
    for (j = 0; j < proc; j++) waitid(P_ALL, 0, NULL, WEXITED);
  }
  goto exit_frc;
  error_frc:
    perror("something wrong");
  exit_frc:
    return;
    for (pi = 0; pi < CONTPROC; pi++) {
      if (unlink(filename[pi]) == -1)
        perror("File remove failed");
    }
}
