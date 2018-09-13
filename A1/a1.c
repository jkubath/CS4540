/*
	Author: Jonah Kubath
	Class: CS4540
	Assignment: A1
	Date: 9/11/2018
*/
#include "a1.h"

char * cmdLine(int argc, char** argv);
FILE * openFile(char * f);
int readFile(FILE * f, peopleList * peopleL, phoneList * phoneL);
char ** splitString(char * line, int n, char * delim);
person * copyPersonData(person * old, char ** data);
peopleList * addPersonToList(peopleList * peopleHead, person * newPerson);
void addPhone(person * newPerson, char * area, char * number);

int main(int argc, char** argv) {
	printf("Running A1\n");

	peopleList * peopleL = NULL;
	phoneList * phoneL = NULL;
	FILE * file = NULL;

	//printf("-%s-\n", cmdLine(argc, argv));
	char * temp = cmdLine(argc, argv);

	file = openFile(temp);

	readFile(file, peopleL, phoneL);


	free(peopleL);
	free(phoneL);
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

int readFile(FILE * f, peopleList * peopleL, phoneList * phoneL) {
	size_t bufferSize = 500;
	char * buffer = calloc(sizeof(char), bufferSize);
	ssize_t numRead;
	char start[] = "<person>";
	char end[] = "</person>";

	peopleList * peopleHead = peopleL;
	phoneList * phoneHead = phoneL;

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

			//Save person to the list
			peopleHead = addPersonToList(peopleHead, newPerson);

			printf("%s\n", peopleHead->person->first);

			//Priming read of the next line
			memset(buffer, 0, bufferSize); //Clear old data
			numRead = getline(&buffer, &bufferSize, f);
			//Read the phone data
			while(strncmp(buffer, end, strlen(end)) != 0){
				printf("Buffer-%s\n", buffer);
				
				returnValues = splitString(buffer, 4, " ");

				addPhone(peopleHead->person, returnValues[0], returnValues[1]);
			
				//Get the next line
				memset(buffer, 0, bufferSize); //Clear old data
				numRead = getline(&buffer, &bufferSize, f);
			}

			//printf("Found\n");
		}
	}

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

	strncpy(old->last, data[2], strlen(data[0]));
	strncpy(old->middle, data[1], strlen(data[1]));
	strncpy(old->first, data[0], strlen(data[2]));
	strncpy(old->nickname, data[3], strlen(data[3]));

	free(data[0]);
	free(data[1]);
	free(data[2]);
	free(data[3]);

	return old;
}

peopleList * addPersonToList(peopleList * peopleHead, person * newPerson) {
	if(peopleHead == NULL){
		printf("First-%s\n", newPerson->first);
		peopleHead = calloc(sizeof(peopleList), 1);
		peopleHead->person = newPerson;
		peopleHead->next = NULL;
	}
	else {
		peopleHead->next = calloc(sizeof(peopleList), 1);
		peopleHead = peopleHead->next;
		peopleHead->person = newPerson;
	}

	return peopleHead;
}

void addPhone(person * newPerson, char * area, char * number) {
	phone * newPhone = NULL;

	if(newPerson->nextPhone == NULL){
		newPerson->nextPhone = calloc(sizeof(phone), 1);
		newPhone = newPerson->nextPhone;
	}
	else {
		newPhone = newPerson->nextPhone;
		while(newPhone->nextPhone != NULL) {
			newPhone = newPhone->nextPhone;
		}
		newPhone->nextPhone = calloc(sizeof(phone), 1);
		newPhone = newPhone->nextPhone;
	}

	newPhone->nextPhone = NULL;

	newPhone->number = calloc(sizeof(char), 
		strlen(area) + strlen(number) + 2);
	sprintf(newPhone->number, "%s %s", area, number);

	printf("Phone-%s\n", newPhone->number);


}




















