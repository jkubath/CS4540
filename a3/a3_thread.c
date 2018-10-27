#include "a3_thread.h"

#define DEBUG 1
#define PRINT_INLINE 1
#define PRINT_SUMMARY 1

int main(int argc, char ** argv) {
  printf("Threads with Deadlock resolution\n");

  semStruct sem_s;
  int threadNumber = 9;
	pthread_t pids[threadNumber]; 
	int i = 0; // Index value

  // Open the semaphores
  init(&sem_s);
  sem_s.id = 0;

  printf("sem\n");
  sem_s.deadlocks = calloc(sizeof(int) * threadNumber, 1);

  /* Open 9 threads
   * -------------------------------
   */
  do {
    // Run the thread
    if(DEBUG) {
      printf("Creating thread %d\n", i);
    }

		if(pthread_create(&(pids[i]), NULL, Thread, (void *) &sem_s) != 0) {
      printf("Error creating thread %d\n", i);
    }

    sem_s.id++;
		i++;
	} while( i < threadNumber);
	
  /* Wait for all threads to finish
   * -------------------------------
  */ 
  i = 0;
  while(i < threadNumber) {
    if(pthread_join(pids[i], NULL)) {
      printf("Error joining thread %d\n", i);
    }
    i++;
  }
  

  /* Close Resources */
  sem_close(&(sem_s.sems));
  sem_close(&(sem_s.semk));
  printf("Parent finished\n");

  if(PRINT_SUMMARY) {
    printSummary(sem_s.deadlocks, threadNumber);
  }

  
	return 0;
} //End main

/*
 * Open the semaphores
 */
void init(semStruct * sem) {
  int returnVal = 0;
  returnVal = sem_init(&(sem->sems), 0, 1);
  /* Error Check */
  if(returnVal != 0) {
    printf("Screen semaphore failed to open\n");
    exit(0);
  }
  returnVal = sem_init(&(sem->semk), 0, 1);
  /* Error Check */
  if(returnVal != 0) {
    printf("Keyboard semaphore failed to open\n");
    exit(0);
  }

  if(DEBUG) {
    printf("Semaphores initialized\n");
  }
}

/*
 * Run the process until the user quits
 */
void * Thread (void * sem_s) {
  semStruct * semaphores = (semStruct *) sem_s;
  int id = semaphores->id;
  int count = 0; // Count of deadlocks for this thread
  int bufferLength = 80;
  char * input = calloc(bufferLength, 1); // User input buffer

  //Allow all threads to be made
  sleep(1);

  if(DEBUG) {
    printf("Thread %d\n", id);
  }

  /* Loop the thread until user quits */
  while(strnlen(input, bufferLength) > 1 || input[0] != 'q') {
    count += semaphore(semaphores, id);

    getUserInput(input, bufferLength, id);
    printf("%d Input: %s\n", id, input);

    /* Release resources */
    sem_post(&(semaphores->sems));
    sem_post(&(semaphores->semk));

  }

  if(PRINT_INLINE) {
	 printf("Thread %d finished with %d deadlock(s)\n", id, count);
  }
  free(input);

  semaphores->deadlocks[id] = count;
  return (void *) NULL;
}

/*
 * Lock the left semaphore and
 * continue to use sem_timedwait to attempt
 * to lock the right semaphore
 */
int semaphore(semStruct * sem_s, int index) {
  //returns count of how many times recovered from deadlock
  //odd index gets screen first
  //even index gets keyboard first
  int count = 0;
  int l = -1;
  int r = -1;
  int sleepTime = 5; // Random sleepTime from 0 - (sleepTime - 1)
  struct timespec ts;
  int waitTime = 1; // Time to wait for right resource to become availabe before failing
  sem_t * left;
  sem_t * right;
  char * lName, * rName;

  //Even proccess ID
  if((index % 2) == 0) {
      left = &(sem_s->semk);
      lName = "keyboard";
      right = &(sem_s->sems);
      rName = "screen";
  }
  // Odd process ID
  else {
      left = &(sem_s->sems);
      lName = "screen";
      right = &(sem_s->semk);
      rName = "keyboard";
  }
    
  // Loop until both semaphores are locked
  while(l != 0 || r != 0) {
      // Always grab the left resource first
      l = sem_wait(left);
      if(DEBUG) {
        printf("%d locked %s\n", index, lName);
      }

      // Attempt to grab the right resource for waitTime seconds
      setTimedWait(&ts, waitTime);
      r = sem_timedwait(right, &ts);

      // Grabbing right resource failed
      if(r != 0) {
        if(DEBUG) {
          printf("%d failed to lock %s\n", index, rName);
        }

        // Release Resources
        sem_post(left);
        // Wait for sleepTime seconds
        sleep(rand() % sleepTime);
        // Increment deadlock counter
        count++;
      }
      else {
        if(DEBUG) {
          printf("%d locked both resources\n", index);
        }
      }
  } // End of loop

  return count;
} //end get semaphores function

/* 
 * Set the wait period in the timespec struct
 * Current absolute time + waitTime (seconds)
 */
void setTimedWait(struct timespec * ts, int waitTime) {
  clock_gettime(CLOCK_REALTIME, ts); //Get current time
  ts->tv_sec += waitTime; //Add waitTime
}

/*
 * Get the user input for the given process and save to
 * the char * buffer
 */
void getUserInput(char * buffer, int length, int processIndex) {
  /* Get user input */
  printf("%d Enter < %d characters or q to quit:\n", processIndex, length);
  /* Input Error Checking */
  if(scanf("%s", buffer) == EOF) {
    printf("read error\n");
    exit(0);
  }
}

/*
 * Iterate and print all the deadlock counts
 */
void printSummary(int deadlocks[], int count) {
  int i = 0;

  while(i < count) {
    printf("%d had %d deadlocks\n", i, deadlocks[i]);
    i++;
  }
}














