#include <stdio.h>			/* For print */
#include <semaphore.h>		/* Semaphores */
#include <unistd.h>			/* Fork, sleep */
#include <stdlib.h>			/* exit, calloc */
#include <string.h>			/* strnlen */
#include <time.h>			/* getcurrenttime */
#include <errno.h>			/* defines errno */
#include <sys/mman.h>		/* shared memory for deadlocks count */
#include <pthread.h>		/* Creates pthread */

typedef struct sem_s {
	sem_t sems;
	sem_t semk;
	int id;
	int * deadlocks;
} semStruct;

void init(semStruct * sem_s);
void * Thread (void * sem_s);
int semaphore (semStruct * sem_s, int index);
void setTimedWait(struct timespec * ts, int waitTime);
void getUserInput(char * buffer, int length, int processIndex);
void printSummary(int deadlocks[], int count);
