
all: a3_process.c a3_process.h a3_thread.c a3_thread.h
	gcc -Wall -O -pthread -pedantic -o a3_process a3_process.c
	gcc -Wall -O -pthread -pedantic -o a3_thread a3_thread.c

process:
	./a3_process

thread:
	./a3_thread

use:
	./a3

val:
	valgrind ./a3