#include "../include/a1.h"

void menu(person * people[], int peopleLength, phone * phone[], int phoneLength) {
	int choice = 0;

	while(choice != 3) {
		printf("1. Search people\n");
		printf("2. Search phones\n");
		printf("3. Quit\n");

		if(fscanf(stdin, "%d", &choice) < 0)
			printf("Scan failed. Try again.\n");
		else {
			printf("You entered: %d\n", choice);
		}

		//Clear buffer
		char * temp = calloc(sizeof(char), 10);
		temp = fgets(temp, 10, stdin);

		if(choice == 1) {
			searchPeople(people, peopleLength);
		}
		else if(choice == 2) {
			searchPhone(phone, phoneLength);
		}
		else if(choice == 3) {
			//Do nothing
		}
		else {
			printf("Choice not found\n");
		}

		if(choice != 3)
			choice = -1;

		free(temp);

	}
}

void searchPeople(person * people[], int peopleLength) {
	printf("%s\n", "Enter the person's name (First Last)");
	char * input = getString(30);

	if(input == NULL) {
		printf("Error reading input\n");
	}

	int index = 0;
	int found = 0;
	while(index < peopleLength && people[index] != NULL && found == 0) {
		char * name = calloc(sizeof(char), 
			strlen(people[index]->first) + strlen(people[index]->last) + 2);
		sprintf(name, "%s %s", people[index]->first, people[index]->last);

		//Compare input to name
		if(strncmp(input, name, strlen(name)) == 0){
			found = 1;

			printf("%s %s %s (%s)\n", people[index]->first, people[index]->middle, people[index]->last, people[index]->nickname);
			printPhoneList(people[index]->nextPhone);
			printf("\n");
		}
		
		free(name);
		index++;
	}

	if(index >= peopleLength || people[index] == NULL) {
		printf("No record found\n");
	}

	free(input);

}

void searchPhone(phone * phone[], int phoneLength) {
	printf("%s\n", "Enter the person's number - (123) 123-4567");
	char * input = getString(30);

	if(input == NULL) {
		printf("Error reading input\n");
	}

	int index = 0;
	int found = 0;
	while(index < phoneLength && phone[index] != NULL && found == 0) {
		char * n = phone[index]->number;
		
		//Compare input to name
		if(strncmp(input, n, strlen(n)) == 0){
			found = 1;

			printf("%s\n", phone[index]->number);
			printPersonList(phone[index]->nextPerson);
			printf("\n");
		}
		
		index++;
	}

	if(index >= phoneLength || phone[index] == NULL) {
		printf("No record found\n");
	}

	free(input);
}
