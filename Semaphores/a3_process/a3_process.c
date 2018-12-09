#include "a3_process.h"

#define DEBUG 0
#define PRINT_INLINE 1
#define PRINT_SUMMARY 0

int main(int argc, char ** argv) {
  printf("Processes with Deadlock resolution\n");
  sem_t * sems = NULL;
	sem_t * semk = NULL;

  int procNumber = 9;
	int pids[procNumber]; 
	int pid; // Holds the PID of the forked process
	int i = 0; // Index value

  // Open the semaphores
  init(&sems, &semk);
  

  /* Open 9 processes
   * -------------------------------
   */
	do {
    // Run the child process
		if( (pid = fork()) == 0) {
      sleep(1); // Wait for all processes to be made
      Process(sems, semk, i);
    }
    // Add it to the parent's array
		else {
      pids[i] = pid;
    }

		i++;
	} while( i < procNumber && pid > 0);
	
  /* Wait for all children to finish
   * -------------------------------
   */
	if(i >= procNumber && pid != 0) {
    i = 0;
    while(i < procNumber) {
      waitpid(pids[i], NULL, 0);
      i++;
    }

    /* Close Resources */
    sem_close(sems);
    sem_close(semk);
    printf("Parent finished\n");

	}

  
	return 0;
} //End main

/*
 * Open the semaphores
 */
void init(sem_t ** screen, sem_t ** keyboard) {
  *screen = sem_open("/screen", O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, 1);
  sem_unlink("/screen"); // Prevent locked resources

  /* Error Check */
  if(*screen == SEM_FAILED) {
    printf("Screen semaphore failed to open\n");
    exit(0);
  }

  *keyboard = sem_open("/keyboard", O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, 1);
  sem_unlink("/keyboard"); // Prevent locked resources

  /* Error Check */
  if(*keyboard == SEM_FAILED) {
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
int Process(sem_t * screen, sem_t * keyboard, int index) {
  int count = 0; // Count of deadlocks for this process
  int bufferLength = 80;
  char * input = calloc(bufferLength, 1); // User input buffer

  /* Loop the process until user quits */
  while(strnlen(input, bufferLength) > 1 || input[0] != 'q') {
    count += semaphores(screen, keyboard, index);

    getUserInput(input, bufferLength, index);
    printf("%d Input: %s\n", index, input);

    /* Release resources */
    sem_post(screen);
    sem_post(keyboard);

  }

  if(PRINT_INLINE) {
	 printf("Process %d finished with %d deadlock(s)\n", index, count);
  }
  free(input);

  return count;
}

/*
 * Lock the left semaphore and
 * continue to use sem_timedwait to attempt
 * to lock the right semaphore
 */
int semaphores(sem_t * screen, sem_t * keyboard, int index) {
  //returns count of how many times recovered from deadlock
  //odd index gets screen first
  //even index gets keyboard first
  int count = 0;
  int l = -1;
  int r = -1;
  int sleepTime = 10; // Random sleepTime from 0 - (sleepTime - 1)
  struct timespec ts;
  int waitTime = 1; // Time to wait for right resource to become availabe before failing
  sem_t * left;
  sem_t * right;
  char * lName, * rName;

  //Even proccess ID
  if((index % 2) == 0) {
      left = keyboard;
      lName = "keyboard";
      right = screen;
      rName = "screen";
  }
  // Odd process ID
  else {
      left = screen;
      lName = "screen";
      right = keyboard;
      rName = "keyboard";
  }
    
  // Loop until both semaphores are locked
  while(l != 0 || r != 0) {
      // Always grab the left resource first
      l = sem_wait(left);
      sleep(1);
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















