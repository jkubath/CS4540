char * cmdLine(int argc, char** argv);
FILE * openFile(char * f);
int readFile(FILE * f, person ** peopleL, int * peopleLength, phone ** phoneL, int * phoneLength);
char ** splitString(char * line, int n, char * delim);
char * getString();