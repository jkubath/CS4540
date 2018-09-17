#include "../include/a1.h"

/* Set the range of values to NULL */
person ** initializePersonList(person * list[], int start, int end) {
	int index = start;

	while(index < end) {
		list[index] = NULL;
		index++;
	}

	return list;
}

/* Set the range of values to NULL */
phone ** initializePhoneList(phone * list[], int start, int end) {
	int index = start;

	while(index < end) {
		list[index] = NULL;
		index++;
	}

	return list;
}