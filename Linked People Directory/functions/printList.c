#include "../include/a1.h"

/* Print the array of people */
void printPeople(person ** list) {
	int index = 0;

	printf("Printing Person List\n");
	printf("--------------------\n");
	if(list == NULL){
		printf("%s\n", "NULL LIST");
	}

	while(list[index] != NULL) {
		person * p = list[index];
		printf("%s %s %s\n", p->first, p->middle,
			p->last);
		printPhoneList(p->nextPhone);

		index++;
	}
}

/* Print the linked list of phones */
void printPhoneList(phoneList * head) {
	while(head != NULL) {
		printf("\t%s\n", head->phoneP->number);
		head = head->next;
	}
}

/* Print the array of phones */
void printPhone(phone ** list) {
	int index = 0;

	printf("Printing Phone List\n");
	printf("-------------------\n");
	if(list == NULL){
		printf("%s\n", "NULL LIST");
	}

	while(list[index] != NULL) {
		phone * p = list[index];
		printf("%s\n", p->number);
		printPersonList(p->nextPerson);

		index++;
	}

}

/* Print the linked list of people */
void printPersonList(peopleList * head) {
	while(head != NULL) {
		printf("\t%s %s\n", head->person->first, head->person->last);
		head = head->next;
	}
}

