#include <stdio.h>			/* For print */
#include <fcntl.h>         	/* For O_* constants */
#include <sys/stat.h>      	/* For mode constants */
#include <semaphore.h>		/* Semaphores */
#include <unistd.h>			/* Fork, sleep */
#include <stdlib.h>			/* exit, calloc */
#include <string.h>			/* strnlen */
#include <time.h>			/* getcurrenttime */
#include <errno.h>			/* defines errno */
#include <sys/wait.h>		/* waitpid */
#include <sys/mman.h>		/* shared memory for deadlocks count */

void init(sem_t ** screen, sem_t ** keyboard);
int Process (sem_t * screen, sem_t * keyboard, int index);
int semaphores (sem_t * screen, sem_t * keyboard, int index);
void setTimedWait(struct timespec * ts, int waitTime);
void getUserInput(char * buffer, int length, int processIndex);
void printSummary(int deadlocks[], int count);
