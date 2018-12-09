
all: a3_thread.c a3_thread.h
	gcc -Wall -O -pthread -pedantic -o a3_thread a3_thread.c

thread:
	./a3_thread

use:
	./a3_thread


val:
	valgrind ./a3_thread
