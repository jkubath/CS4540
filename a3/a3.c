#include "a3.h"

struct timespec ts;


int main(int argc, char ** argv) {
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 1;

	sem_t * sems;
	sem_t * semk;
  sem_unlink("screen");
  sem_unlink("keybaord");
	sems = sem_open("screen", O_CREAT|O_EXCL, 0644, 0);
  if(errno == SEM_FAILED) {
    printf("Screen semaphore failed to open\n");
    exit(1);
  }
	semk = sem_open("keyboard", O_CREAT|O_EXCL, 0644, 0);
  if(errno == SEM_FAILED) {
    printf("Keyboard semaphore failed to open\n");
    exit(1);
  }

  int procNumber = 1;
	int pids[procNumber]; 
	int pid; // Holds the PID of the forked process
	int i = 0; // Index value
  

  /* Open 9 processes */
	do {
    //Run the child process
		if( (pid = fork()) == 0) {
      printf("Process %d made\n", i);
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

    // sem_unlink both 
    sem_unlink("screen");
    sem_unlink("keybaord");
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
    scanf("%s\n", input);
    printf("%d Input: %s\n", index, input);

  }

  sem_post(screen);
  sem_post(keyboard);
	// until quit end inner loop in C do ... while not q
	//    count += getSemaphores(index)
	//      prompt "This process had " + count + " deadlocks "
	//    sem_post or give back both semaphores
	// sem_close 
  sem_close(screen);
  sem_close(keyboard);
	printf("Process %d finished with %d deadlocks\n", index, count);


}

int semaphores(sem_t * screen, sem_t * keyboard, int index) {
  //returns count of how many times recovered from deadlock
  //odd index gets screen first
  //even index gets keyboard first
  int count = 0;
  int s = -1;
  int k = -1;
  printf("Semaphores %d\n", index);

  // Even
  if((index % 2) == 0) {
    printf("even\n");
    k = sem_wait(keyboard);
    printf("%d locked keyboard\n", index);
  }
  // Odd
  else {
    printf("odd\n");
    s = sem_wait(screen);
    printf("%d locked screen\n", index);
  }
  	
  // Loop until both semaphores are locked
  while(s != 0 || k != 0) {
    printf("%d while\n", index);
    // Even
    if((index % 2) == 0) {
      s = sem_timedwait(screen, &ts);
      if(s != 0) {
        printf("%d failed to lock screen\n", index);
        sem_post(keyboard);
        sleep(1);
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
        sleep(1);
        count++;
      }
      else {
        printf("%d locked keyboard\n", index);
      }
    }

  }
  

 	return count;
} //end get semaphores function














