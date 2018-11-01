#include "../include/a1.h"

/* iterate the person and phone arrays to free the objects
 * contained in them */
void freeThings(person ** pe, phone ** ph) {
	int index = 0;
	person * tPerson = pe[index];
	phone * tPhone = ph[index];

	//Free the person array
	while(tPerson != NULL) {
		free(tPerson->first);
		free(tPerson->middle);
		free(tPerson->last);
		free(tPerson->nickname);

		phoneList * list = tPerson->nextPhone;
		phoneList * hold = list;
		while(list != NULL) {
			hold = hold->next;
			free(list);
			list = hold;
		}

		index++;
		tPerson = pe[index];
	}

	index = 0;

	//Free the phone array
	while(tPhone != NULL) {
		free(tPhone->number);

		peopleList * list = tPhone->nextPerson;
		peopleList * hold = list;
		while(list != NULL) {
			hold = list->next;
			free(list);
			list = hold;
		}

		index++;
		tPhone = ph[index];
	}

}
