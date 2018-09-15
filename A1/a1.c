/*
	Author: Jonah Kubath
	Class: CS4540
	Assignment: A1
	Date: 9/11/2018
*/
#include "a1.h"

char * cmdLine(int argc, char** argv);
FILE * openFile(char * f);
int readFile(FILE * f, person ** peopleL, phone ** phoneL);
char ** splitString(char * line, int n, char * delim);
person * copyPersonData(person * old, char ** data);
//void addPersonToList(peopleList ** peopleHead, person * newPerson, int personLength);
int addPersonToList(person ** list, person * newPerson, int personLength);
int addPhoneToList(phone ** list, phone * p, int phoneLength);
void addPhone(person * newPerson, char ** data);
void printPeople(person ** list);
void printPhoneList(phoneList * head);
person ** initializePersonList(person ** list, int start, int end);
phone ** initializePhoneList(phone ** list, int start, int end);
int copyPhoneData(phone ** phoneList, person * newPerson, int phoneLength);
void addPerson(phone * newPhone, person * newPerson);
void printPhone(phone ** list);
void printPersonList(peopleList * head);

int main(int argc, char** argv) {
	printf("Running A1\n");

	// peopleList * peopleL = NULL;
	// phoneList * phoneL = NULL;
	int length = 50;
	person ** person_L = calloc(sizeof(person), length);
	phone ** phone_L = calloc(sizeof(phone), length);
	FILE * file = NULL;

	initializePersonList(person_L, 0, length);

	//printf("-%s-\n", cmdLine(argc, argv));
	char * temp = cmdLine(argc, argv);

	file = openFile(temp);

	readFile(file, person_L, phone_L);


	// free(peopleL);
	// free(phoneL);
	printf("Done A1\n");
	return 0;
}

char * cmdLine(int argc, char** argv) {
	char * returnString = NULL;

	if(argc > 1)
		returnString = argv[1];

	return returnString;
}

FILE * openFile(char * f) {
	FILE * file = NULL;
	int stringLength = 20;
	char * newFile = calloc(stringLength * sizeof(char), 1);

	if(f != NULL)
		strncpy(newFile, f, stringLength);

	while((file = fopen(newFile, "r")) == NULL) {
		printf("%s file not found\n", newFile);
		printf("Enter a new file\n");

		memset(newFile, 0, stringLength); //Clear old data
		newFile = fgets(newFile, stringLength, stdin); //get new characters

		//User entered too many characters
		if(strnlen(newFile, stringLength) >= stringLength - 1) {
			printf("Enter only %d characters\n", stringLength);
			while( getchar() != '\n') {
				//consume the rest of the stdin buffer
			}
		}
		//Replace the \n from fgets to \0
		else {
			int index = 0;
			while(index <= stringLength && newFile[index] != '\n'){
				index++;
			}
			newFile[index] = '\0';
		}
	}

	free(newFile);
	return file;

}

int readFile(FILE * f, person ** peopleL, phone ** phoneL) {
	size_t bufferSize = 500;
	char * buffer = calloc(sizeof(char), bufferSize);
	ssize_t numRead;
	char start[] = "<person>";
	char end[] = "</person>";

	person ** peopleHead = peopleL;
	phone ** phoneHead = phoneL;
	int personLength = 50;
	int phoneLength = 50;

	while((numRead = getline(&buffer, &bufferSize, f)) != -1){
		//printf("%s", buffer);

		//Create person, phone objects
		if(strncmp(buffer, start, strlen(start)) == 0) {
			//Create a new person
			person * newPerson = calloc(sizeof(person), 1);

			//Get the next line
			memset(buffer, 0, bufferSize); //Clear old data
			numRead = getline(&buffer, &bufferSize, f);

			//printf("%s", buffer);
			char ** returnValues = splitString(buffer, 4, ",");

			newPerson = copyPersonData(newPerson, returnValues);

			//Priming read of the next line
			memset(buffer, 0, bufferSize); //Clear old data
			numRead = getline(&buffer, &bufferSize, f);
			//Read the phone data
			while(strncmp(buffer, end, strlen(end)) != 0){
				//printf("p-%s\n", buffer);
				
				returnValues = splitString(buffer, 4, " ");

				addPhone(newPerson, returnValues);
			
				//Get the next line
				memset(buffer, 0, bufferSize); //Clear old data
				numRead = getline(&buffer, &bufferSize, f);
			}

			//Save person to the list
			personLength = addPersonToList(peopleHead, newPerson, personLength);
			//Save number to the list
			phoneLength = copyPhoneData(phoneHead, newPerson, phoneLength);
			//printf("Add to list\n");
			//printf("Found\n");
			//printf("head-%s\n", peopleHead->person->first);
		}
	}

	printPeople(peopleHead);
	printPhone(phoneHead);

	free(buffer);
	return 0;
}

char ** splitString(char * line, int n, char * delim) {
	char ** broken = calloc(sizeof(char *), n); //Freed in copyPersonData()
	char * token = NULL;
	char * temp = NULL;
	int index = 0;


	token = strtok_r(line, delim, &temp);
	broken[index] = calloc(sizeof(char) * strlen(token), 1);
	strncpy(broken[index], token, strlen(token));
	index++;
	//printf("Token-%s\n", token);

	while((token = strtok_r(NULL, delim, &temp)) != NULL){
		//printf("Token-%s\n", token);

		broken[index] = calloc(sizeof(char) * strlen(token), 1);
		strncpy(broken[index], token, strlen(token));

		index++;
	}

	return broken;
}

person * copyPersonData(person * old, char ** data) {
	old->last = calloc(sizeof(char), strlen(data[0]));
	old->middle = calloc(sizeof(char), strlen(data[1]));
	old->first = calloc(sizeof(char), strlen(data[2]));
	old->nickname = calloc(sizeof(char), strlen(data[3]));

	strncpy(old->last, data[0], strlen(data[0]));
	strncpy(old->middle, data[1] + 1, strlen(data[1]) - 1);
	strncpy(old->first, data[2] + 1, strlen(data[2]) - 1);
	strncpy(old->nickname, data[3] + 1, strlen(data[3]) - 1);

	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data[3]);

	return old;
}

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

	//printf("Phone-%s\n", newPhone->phoneP->number);
}

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

	list[index] = p;

	return (phoneLength * change);
}

person ** initializePersonList(person ** list, int start, int end) {
	int index = start;

	while(index < end) {
		list[index] = NULL;
		index++;
	}

	return list;
}

phone ** initializePhoneList(phone ** list, int start, int end) {
	int index = start;

	while(index < end) {
		list[index] = NULL;
		index++;
	}

	return list;
}

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

	//printf("print done\n");
}

void printPhoneList(phoneList * head) {
	while(head != NULL) {
		printf("\t%s\n", head->phoneP->number);
		head = head->next;
	}
}

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

	//printf("print done\n");
}

void printPersonList(peopleList * head) {
	while(head != NULL) {
		printf("\t%s %s\n", head->person->first, head->person->last);
		head = head->next;
	}
}




















