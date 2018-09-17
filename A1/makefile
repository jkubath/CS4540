
all: a1.c functions/person.c functions/phone.c functions/initializeList.c functions/file.c functions/printList.c functions/freeThings.c functions/interaction.c
	gcc -Wall -O -pedantic -o a1 a1.c functions/person.c functions/phone.c functions/initializeList.c functions/file.c functions/printList.c functions/freeThings.c functions/interaction.c

use:
	./a1 a1in.txt

use1:
	./a1 input.txt

test:
	./a1 input.txt < userInput.txt

val:
	valgrind ./a1 a1in.txt