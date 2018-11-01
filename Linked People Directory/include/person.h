typedef struct person_s {
	char * first;
	char * middle;
	char * last;
	char * nickname;
	
	struct phoneList_s * nextPhone;
	//struct person_s * nextPerson;
} person;

person * copyPersonData(person * old, char ** data);
int addPersonToList(person ** list, person * newPerson, int personLength);
void addPhone(person * newPerson, char ** data);
