typedef struct phone_s {
	char * number;

	//struct phone_s * nextPhone;
	struct peopleList_s * nextPerson;
} phone;

int addPhoneToList(phone ** list, phone * p, int phoneLength);
int copyPhoneData(phone ** phoneList, person * newPerson, int phoneLength);
void addPerson(phone * newPhone, person * newPerson);
