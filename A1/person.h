typedef struct person_s {
	char * first;
	char * middle;
	char * last;
	char * nickname;
	
	struct phone_s * nextPhone;
	//struct person_s * nextPerson;
} person;
