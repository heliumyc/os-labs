CFLAGS:=-g # -g = debug, -O2 for optimized code CPPFLAGS for g++
CC:=g++
SHELL:=/bin/bash
SRCS := $(wildcard *.h *.cpp ./Schedulers/*.h ./Schedulers/*.cpp)
sched: $(SRCS)
	module unload; \
	module load gcc-9.1; \
	gcc -v; \
	$(CC) $(CFLAGS) $(SRCS) -std=c++17 -o sched
testgcc:
	gcc -v; \
	module load gcc-9.1; \
	gcc -v;