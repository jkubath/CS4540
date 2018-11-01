
all: a3_process.c a3_process.h
	gcc -Wall -O -pthread -pedantic -o a3_process a3_process.c

process:
	./a3_process


use:
	./a3_process


val:
	valgrind ./a3_process
