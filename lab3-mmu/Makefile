CFLAGS:=-g # -g = debug, -O2 for optimized code CPPFLAGS for g++
CC:=g++
SHELL:=/bin/bash
GCCVER=9.1
SRCS := $(wildcard *.h *.cpp ./Pagers/*.h ./Pagers/*.cpp)
mmu: $(SRCS)
	module unload; \
	module load gcc-$(GCCVER); \
	gcc -v; \
	$(CC) $(CFLAGS) $(SRCS) -std=c++17 -o mmu
testgcc:
	gcc -v; \
	module load gcc-$(GCCVER); \
	gcc -v;