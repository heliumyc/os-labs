CFLAGS=-g # -g = debug, -O2 for optimized code CPPFLAGS for g++
CC=g++
SHELL:=/bin/bash
linker:  main.cpp tokenizer.h tokenizer.cpp parser.h parser.cpp errors.h errors.cpp
	module unload; \
	module load gcc-6.3.0; \
	gcc -v; \
	$(CC) $(CFLAGS) main.cpp tokenizer.h tokenizer.cpp parser.h parser.cpp errors.h errors.cpp -std=c++17 -o linker
testgcc:
	gcc -v; \
	module load gcc-7.4; \
	gcc -v;