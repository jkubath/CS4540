
#include "a4.h"




int main() {
	signal(SIGINT, handleSignal);
	signal(SIGALRM, handleAlarm);

	printf("Signal Handling\n");
	alarm(3);
	sleep(20);

	printf("Sleep Interrupted\n");

	alarm(2);
	sleep(20);
	printf("Sleep Interrupted");

	pause();
	printf("Pause Interrupted");

	printf("Use kill -9 $(pgrep terminal) to end the process\n");
	
	while(1) {

	}



}

void handleSignal() {
	printf("CTRL-C caught\n");
}

void handleAlarm() {
	printf("Alarm caught\n");
}