
#include "a4.h"




int main() {
	signal(SIGINT, handleSignal);
	signal(SIGALRM, handleAlarm);

	int sleepTime = 20;
	int sleepRet = 0;

	printf("Signal Handling\n");

	alarm(3);

	sleepRet = sleep(sleepTime);
	if(sleepRet != sleepTime) {
		printf("Sleep interrupted with %ds left\n", sleepRet);
	}


	alarm(2);

	sleepRet = sleep(20);
	if(sleepRet != sleepTime) {
		printf("Sleep interrupted with %ds left\n", sleepRet);
	}

	pause();

	printf("Signal Handling finished\n");
	
	return 0;
}

void handleSignal() {
	printf("\nCTRL-C caught\n");
}

void handleAlarm() {
	printf("Alarm caught\n");
}