#include "a3.h"

struct timespec ts;

int globalInt = 0;


int main(int argc, char ** argv) {
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 1;

	sem_t * sems;
	sem_t * semk;

  sems = sem_open("/screen", O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, 1);
  //sem_unlink("/screen");
  
  /* Error Check */
  if(sems == SEM_FAILED) {
    printf("Screen semaphore failed to open\n");
    exit(0);
  }

  semk = sem_open("/keyboard", O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, 1);
  //sem_unlink("/keyboard");

  /* Error Check */
  if(semk == SEM_FAILED) {
    printf("Keyboard semaphore failed to open\n");
    exit(0);
  }

  printf("Semaphores initialized\n");


  int procNumber = 9;
	int pids[procNumber]; 
	int pid; // Holds the PID of the forked process
	int i = 0; // Index value
  

  /* Open 9 processes */
	do {
    //Run the child process
		if( (pid = fork()) == 0) {
      printf("Process %d made\n", i);
      sleep(1);
			Process(sems, semk, i);
    }
    //Add it to the parent's array
		else {
      pids[i] = pid;
    }

		i++;
	} while( i < procNumber && pid > 0);
	
  //wait for all children to finish
	if(i >= procNumber && pid != 0) {
    i = 0;
    while(i < procNumber) {
      waitpid(pids[i], NULL, 0);
      i++;
    }

    printf("Parent finished\n");
	}

  
	return 0;
} //End main


void Process(sem_t * screen, sem_t * keyboard, int index) {
  int count = 0; // Count of deadlocks for this process
  char * input = calloc(80, 1);

  printf("Process %d running\n", index);
	
  while(strnlen(input, 80) > 1 || input[0] != 'q') {
    count += semaphores(screen, keyboard, index);

    printf("%d Enter < 80 characters or q to quit:\n", index);
    if(scanf("%s", input) == EOF) {
      printf("read error\n");
    }
    printf("%d Input: %s\n", index, input);

  }

  //Close the semaphores
  sem_close(screen);
  sem_close(keyboard);
	printf("Process %d finished with %d deadlocks\n", index, count);


}

// int semaphores(sem_t * screen, sem_t * keyboard, int index) {
//     printf("Semaphores\n");
//     sem_wait(keyboard);
//     sem_wait(screen);
//     printf("%d locked keyboard, screen\n", index);
//     sleep(10);
    
//     sem_post(keyboard);
//     sem_post(screen);

//     return 0;
// }

int semaphores(sem_t * screen, sem_t * keyboard, int index) {
  //returns count of how many times recovered from deadlock
  //odd index gets screen first
  //even index gets keyboard first
  int count = 0;
  int s = -1;
  int k = -1;
  int sleepTime = 10;
  printf("Semaphores %d\n", index);

  // Even
  if((index % 2) == 0) {
    k = sem_wait(keyboard);
    printf("%d locked keyboard\n", index);
    sleep(sleepTime);
  }
  // Odd
  else {
    s = sem_wait(screen);
    printf("%d locked screen\n", index);
    sleep(sleepTime);
  }
    
  // Loop until both semaphores are locked
  while(s != 0 || k != 0) {
    // Even
    if((index % 2) == 0) {
      s = sem_timedwait(screen, &ts);
      if(s != 0) {
        printf("%d failed to lock screen\n", index);
        sem_post(keyboard);
        sleep(sleepTime);
        count++;
      }
      else {
        printf("%d locked screen\n", index);
      }
    }
    // Odd
    else {
      k = sem_timedwait(keyboard, &ts);
      if(s != 0) {
        printf("%d failed to lock keyboard\n", index);
        sem_post(screen);
        sleep(sleepTime);
        count++;
      }
      else {
        printf("%d locked keyboard\n", index);
      }
    }

  }

  // globalInt += 1;
  // sleep(1);
  // printf("  %d globalInt\n", globalInt);

  sem_post(screen);
  sem_post(keyboard);
  

   return count;
} //end get semaphores function














