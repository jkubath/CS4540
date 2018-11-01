#include "../include/a1.h"

/* Add a person object to a phone */
void addPerson(phone * newPhone, person * newPerson) {
	peopleList * temp = newPhone->nextPerson;

	//Iterate to the end of the list
	while(temp->next != NULL) {
		temp = temp->next;
	}

	//Create the object in the personList
	temp->next = calloc(sizeof(peopleList), 1);
	temp = temp->next;
	temp->person = newPerson;
	temp->next = NULL;
}

/* Add a phone object to the phone array */
int addPhoneToList(phone ** list, phone * p, int phoneLength) {
	int index = 0; //Index for list
	int change = 1; //If reallocation is needed = 2
	int alreadyAdded = 0; //If the phone number is already in the list = 1
	
	while(alreadyAdded == 0 && index < phoneLength && list[index] != NULL){
		//Phone number has already been added to the list
		if(strncmp(p->number, list[index]->number, strlen(list[index]->number)) == 0){
			addPerson(list[index], p->nextPerson->person);
			alreadyAdded = 1;
		}
		
		index++;
	}

	if(alreadyAdded == 0 && index == phoneLength) {
		list = realloc(list, sizeof(phone) * (phoneLength * 2));
		initializePhoneList(list, phoneLength, phoneLength * 2);
		change = 2;
	}

	if(alreadyAdded == 0)
		list[index] = p;

	return (phoneLength * change);
}

/* Add all the phones for the person to the phone array
 * Also, link that person object to those phone objects */
int copyPhoneData(phone ** phoneList, person * newPerson, int phoneLength) {
	struct phoneList_s * tempPhone = newPerson->nextPhone;
	int returnValue = phoneLength;

	while(tempPhone != NULL) {
		char * number = tempPhone->phoneP->number;
		phone * p = calloc(sizeof(phone), 1);
		p->number = calloc(strlen(number) * sizeof(char), 1);
		strncpy(p->number, number, strlen(number));

		p->nextPerson = calloc(sizeof(peopleList), 1);
		p->nextPerson->person = newPerson;
		p->nextPerson->next = NULL;

		returnValue = addPhoneToList(phoneList, p, phoneLength);

		tempPhone = tempPhone->next;
	}

	return returnValue;
}
