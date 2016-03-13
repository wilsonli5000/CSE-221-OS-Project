/**********
 * netbm.h
 * .h file for network benchmark
 ***********/

#define LOOPTIMES 1
#define TRANSTIMES 4
#define DATASIZE 1*1024*1024 //10*1024*1024

#define MAXLINE DATASIZE //1*1024
#define SERV_PORT 8000

typedef unsigned long long cpu_cycle;

static inline cpu_cycle rdtsc(void) {
	unsigned hi, lo; 
	asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((cpu_cycle) lo) | (((cpu_cycle) hi) << 32);
}
