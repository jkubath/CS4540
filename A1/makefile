
all: a1.c
	gcc -Wall -O -pedantic a1.c -o a1

use:
	./a1 input.txt

val:
	valgrind --leak-check=full --show-leak-kinds=all ./a1 input.txt