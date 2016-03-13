#include <stdio.h>
#include "bench.h"

#define LOOP_TIMES 100
#define DATA_SIZE 16
#define STRIDE 1

#define DO_SIXTEEN_TIMES(p) p;p;p;p;p;p;p;p;p;p;p;p;p;p;p;p;
#define DO_READ(i) n=array[i]; i += STRIDE;
#define DO_WRITE(i) array[i]=n; n.data[0]++; i += STRIDE;

typedef struct _dataType {
	unsigned char data[DATA_SIZE];
}dataType;

void benchmark_ram_bw() {
	cpu_cycle ts1, ts2;
	dataType n, array[256];
	int i, j;
	printf("read begin");
	for (j = 0,i=0; j < LOOP_TIMES; j++, i=0) {
		ts1 = rdtsc();
		DO_SIXTEEN_TIMES(DO_SIXTEEN_TIMES(DO_READ(i)));
		ts2 = rdtsc();
		fprintf(stdout, "Read %d bytes with stride %d for 256 times = %llu\n", 
			DATA_SIZE, STRIDE, ts2-ts1);
	}
	fprintf(stderr, "RAM read BW complete, check output \n");

	for (j = 0,i=0; j < LOOP_TIMES; j++, i=0) {
		ts1 = rdtsc();
		DO_SIXTEEN_TIMES(DO_SIXTEEN_TIMES(DO_WRITE(i)));
		ts2 = rdtsc();
		fprintf(stdout, "Write %d bytes with stride %d for 256 times = %llu\n",
			DATA_SIZE, STRIDE, ts2-ts1);
	}
	fprintf(stderr, "RAM write BW complete, check output \n");
 } 
