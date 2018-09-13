enum phoneType_e {
	Home,
	Cell,
	School
};

typedef struct phoneList_s {
	phone * phoneP;
	enum phoneType_e phoneType;
	bool isPrimary;

	struct phoneList_s * next; 

} phoneList;
