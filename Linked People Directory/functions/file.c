#include "../include/a1.h"

/* Get the filename from argv */
char * cmdLine(int argc, char** argv) {
	char * returnString = NULL;

	if(argc > 1) {
		returnString = argv[1];
	}

	return returnString;
}

/* Open the file or prompt user for new name */
FILE * openFile(char * f) {
	FILE * file = NULL;
	int stringLength = 20;
	char * newFile = calloc(sizeof(char), stringLength);

	if(f != NULL) {
		newFile = calloc(sizeof(char), stringLength);
		strncpy(newFile, f, stringLength);
	}

	while((file = fopen(newFile, "r")) == NULL) {
		printf("%s file not found\n", newFile);
		printf("Enter a new file\n");

		memset(newFile, 0, stringLength); //Clear old data
		newFile = getString(stringLength);
		
	}

	free(newFile);
	return file;
}

char * getString(int stringLength) {
	char * newFile = calloc(sizeof(char), stringLength);
	newFile = fgets(newFile, stringLength - 1, stdin);
	
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

	return newFile;
}

/* Read the contents of the file and create objects */
int readFile(FILE * f, person ** peopleL,int * peopleLe, phone ** phoneL, int * phoneLe) {
	size_t bufferSize = 500;
	char * buffer = calloc(sizeof(char), bufferSize);
	ssize_t numRead;
	char start[] = "<person>";
	char end[] = "</person>";

	person ** peopleHead = peopleL;
	phone ** phoneHead = phoneL;
	int personLength = *peopleLe;
	int phoneLength = *phoneLe;

	while((numRead = getline(&buffer, &bufferSize, f)) != -1){
		//Create person, phone objects
		if(strncmp(buffer, start, strlen(start)) == 0) {
			//Create a new person
			person * newPerson = calloc(sizeof(person), 1);

			//Get the next line
			memset(buffer, 0, bufferSize); //Clear old data
			numRead = getline(&buffer, &bufferSize, f);

			char ** returnValues = splitString(buffer, 4, ",");

			newPerson = copyPersonData(newPerson, returnValues);

			free(returnValues[0]);
			free(returnValues[1]);
			free(returnValues[2]);
			free(returnValues[3]);
			free(returnValues);

			//Priming read of the next line
			memset(buffer, 0, bufferSize); //Clear old data
			numRead = getline(&buffer, &bufferSize, f);
			//Read the phone data
			while(strncmp(buffer, end, strlen(end)) != 0){
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
		}
	}

	

	free(buffer);
	return 0;
}

/* Split a string and return the values in an array */
char ** splitString(char * line, int n, char * delim) {
	char ** broken = calloc(sizeof(char *), n); //Freed in copyPersonData()
	char * token = NULL;
	char * temp = NULL;
	int index = 0;


	token = strtok_r(line, delim, &temp);
	broken[index] = calloc(sizeof(char), strlen(token));
	strncpy(broken[index], token, strlen(token));
	index++;

	while((token = strtok_r(NULL, delim, &temp)) != NULL){
		
		broken[index] = calloc(sizeof(char), strlen(token));
		strncpy(broken[index], token, strlen(token));

		index++;
	}

	return broken;
}