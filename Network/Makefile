CC=gcc
BIN_DIR=bin
SRC_DIR=src
CFLAG= -O0 -Wall -Winline -fno-omit-frame-pointer -funroll-loops
LINKFLAG= -lpthread
RESULT_DIR=results

LIBS=$(BIN_DIR)/measurement.o\
		 $(BIN_DIR)/proc_call.o\
		 $(BIN_DIR)/sys_call.o\
		 $(BIN_DIR)/cr_proc.o\
		 $(BIN_DIR)/cr_thread.o\
		 $(BIN_DIR)/memory_access_time.o\
		 $(BIN_DIR)/filesystem.o\
		 $(BIN_DIR)/context_switch.o\
		 $(BIN_DIR)/ram_bw.o\
		 $(BIN_DIR)/page_fault.o

LIBS_DEP=measurement.o\
				 proc_call.o\
				 sys_call.o\
				 cr_proc.o\
				 cr_thread.o\
					memory_access_time.o\
					filesystem.o\
				 context_switch.o\
				 ram_bw.o\
				 page_fault.o

ifneq ($(DEBUG),)
	CFLAGS += -g -DDEBUG
endif

all: clean benchmark

pre-compile: 
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi

benchmark: $(LIBS_DEP) $(SRC_DIR)/benchmark.c
	$(CC) -o $(BIN_DIR)/benchmark $(LIBS) $(SRC_DIR)/benchmark.c $(CFLAG) $(LINKFLAG)

measurement.o: pre-compile $(SRC_DIR)/measurement.c
	$(CC) -o $(BIN_DIR)/measurement.o -c $(SRC_DIR)/measurement.c $(CFLAG)

proc_call.o: pre-compile $(SRC_DIR)/proc_call.c
	$(CC) -o $(BIN_DIR)/proc_call.o -c $(SRC_DIR)/proc_call.c $(CFLAG)

sys_call.o: pre-compile $(SRC_DIR)/sys_call.c
	$(CC) -o $(BIN_DIR)/sys_call.o -c $(SRC_DIR)/sys_call.c $(CFLAG)

cr_proc.o: pre-compile $(SRC_DIR)/cr_proc.c
	$(CC) -o $(BIN_DIR)/cr_proc.o -c $(SRC_DIR)/cr_proc.c $(CFLAG)

cr_thread.o: pre-compile $(SRC_DIR)/cr_thread.c
	$(CC) -o $(BIN_DIR)/cr_thread.o -c $(SRC_DIR)/cr_thread.c $(CFLAG)

context_switch.o: pre-compile $(SRC_DIR)/context_switch.c
	$(CC) -o $(BIN_DIR)/context_switch.o -c $(SRC_DIR)/context_switch.c $(CFLAG)

memory_access_time.o: pre-compile $(SRC_DIR)/memory_access_time.c
	$(CC) -o $(BIN_DIR)/memory_access_time.o -c $(SRC_DIR)/memory_access_time.c $(CFLAG)

filesystem.o: pre-compile $(SRC_DIR)/filesystem.c
	$(CC) -o $(BIN_DIR)/filesystem.o -c $(SRC_DIR)/filesystem.c $(CFLAG)
ram_bw.o: pre-compile $(SRC_DIR)/ram_bw.c
	$(CC) -o $(BIN_DIR)/ram_bw.o -c $(SRC_DIR)/ram_bw.c $(CFLAG)

page_fault.o: pre-compile $(SRC_DIR)/page_fault.c
	$(CC) -o $(BIN_DIR)/page_fault.o -c $(SRC_DIR)/page_fault.c $(CFLAG)

run_benchmark: benchmark
	script/run_benchmark.sh

set_performance:
	echo performance > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor

run_analysis:
	script/analysis/analysis.py

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(RESULT_DIR)
