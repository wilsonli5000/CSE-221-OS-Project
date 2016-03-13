#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include "bench.h"

#define FILL_TIMES 0
#define LOOP_TIMES 256
#define STEP 1024*1024

void dommap(int fd, int size) {
	char *bufp;
	int i;
	cpu_cycle t1, t2;
	bufp = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (bufp == (void *)-1) {
		fprintf(stderr, "mmap error: %s \n", strerror(errno));
		exit(1);
	}
	for (i = 0; i < FILL_TIMES; i++) {
		bufp[i*STEP] = 'a';
	}
	printf("fill memory complete\n");

	// Start measuring
	for (i = FILL_TIMES; i < FILL_TIMES+LOOP_TIMES; i++) {
		t1 = rdtsc();
		bufp[i*STEP] = 'a';
		t2 = rdtsc();
		printf("page fault time %llu\n", t2-t1);
	}
	munmap(bufp, size);
}
void benchmark_page_fault() {
	int fd, pagesize;
	struct stat stat;
	//system("fallocate -l 1G file.txt");
	if ((fd = open("file.txt", O_RDWR, 0)) < 0)
		fprintf(stderr, "open file error\n");
	fstat(fd, &stat);
	printf("file size = %lld\n", stat.st_size);
	pagesize = getpagesize();
	printf("page size = %d\n", pagesize);

	dommap(fd, stat.st_size);
	close(fd);
	//system("rm file.txt");
}
