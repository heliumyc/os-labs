CFLAGS:=-O2 # -g = debug, -O2 for optimized code CPPFLAGS for g++
CC:=g++
SHELL:=/bin/bash
GCCVER=9.1
SRCS := $(wildcard *.h *.cpp ./IO_Schedulers/*.h ./IO_Schedulers/*.cpp)
iosched: $(SRCS)
	module unload; \
	module load gcc-$(GCCVER); \
	$(CC) $(CFLAGS) $(SRCS) -std=c++17 -o iosched
clean:
	rm -f iosched;
	rm -f *.o;