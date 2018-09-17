/*
	Author: Jonah Kubath
	Class: CS4540
	Assignment: A1
	Date: 9/11/2018
*/
#include "include/a1.h"

int main(int argc, char** argv) {
	//printf("Running A1\n");

	int peopleLength = 50;
	int phoneLength = 50;
	person * person_L[peopleLength];
   	phone * phone_L[phoneLength];
	FILE * file = NULL;

	initializePersonList(person_L, 0, peopleLength);
	initializePhoneList(phone_L, 0, phoneLength);

	char * temp = cmdLine(argc, argv);

	file = openFile(temp);

	readFile(file, person_L, &peopleLength, phone_L, &phoneLength);

	// printPeople(person_L);
	// printPhone(phone_L);

	menu(person_L, peopleLength, phone_L, phoneLength);

	freeThings(person_L, phone_L);
	fclose(file);

	//printf("Done A1\n");
	return 0;
}





















