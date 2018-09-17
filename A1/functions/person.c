#include "../include/a1.h"

/* Copy seperated string data to person object */
person * copyPersonData(person * old, char ** data) {
	old->last = calloc(sizeof(char), strlen(data[0]));
	old->middle = calloc(sizeof(char), strlen(data[1]));
	old->first = calloc(sizeof(char), strlen(data[2]));
	old->nickname = calloc(sizeof(char), strlen(data[3]));

	strncpy(old->last, data[0], strlen(data[0]));
	strncpy(old->middle, data[1] + 1, strlen(data[1]) - 1);
	strncpy(old->first, data[2] + 1, strlen(data[2]) - 1);
	strncpy(old->nickname, data[3] + 1, strlen(data[3]) - 2);

	return old;
}

/* Add person object to array */
int addPersonToList(person ** list, person * newPerson, int personLength) {
	int index = 0;
	int change = 1;

	while(index < personLength && list[index] != NULL){
		index++;
	}

	if(index == personLength) {
		list = realloc(list, sizeof(person) * (personLength * 2));
		initializePersonList(list, personLength, personLength * 2);
		change = 2;
	}

	list[index] = newPerson;

	return (personLength * change);
}

/* Add phone data to a person object */
void addPhone(person * newPerson, char ** data) {
	phoneList * newPhone = NULL;
	if(newPerson->nextPhone == NULL){
		newPerson->nextPhone = calloc(sizeof(phoneList), 1);
		newPhone = newPerson->nextPhone;
	}
	else {
		newPhone = newPerson->nextPhone;
		while(newPhone->next != NULL) {
			newPhone = newPhone->next;
		}
		newPhone->next = calloc(sizeof(phoneList), 1);
		newPhone = newPhone->next;
	}

	newPhone->next = NULL;
	newPhone->phoneP = calloc(sizeof(phone), 1);

	phone * nPhone = newPhone->phoneP;
	nPhone->number = calloc(sizeof(char), 
		strlen(data[0]) + strlen(data[1]) + 2);
	sprintf(nPhone->number, "%s %s", data[0], data[1]);

	newPhone->phoneP = nPhone;
	newPhone->phoneType =(enum phoneType_e) data[2];
	newPhone->isPrimary = data[3];

	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data[3]);
	free(data);

}
