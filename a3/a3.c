#include "a3.h"

// struct timespec {
//     time_t tv_sec;      /* Seconds */
//     long   tv_nsec;     /* Nanoseconds [0 .. 999999999] */
// };

struct timespec ts;


int main(int argc, char ** argv) {
  ts.tv_sec = 1;
	sem_t * sems;
	sem_t * semk;
	sems = sem_open("screen", O_CREAT);
	semk = sem_open("keyboard", O_CREAT);
	int pids[9]; 
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
	} while( i < 9 && pid > 0);
	
  //wait for all children to finish
	if(i >= 9 && pid != 0) {
    for(i = 0; i < 9; i++) {
      waitpid(pids[i], NULL, 0);
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
	
  while(strnlen(input, 80) > 1 || input[0] != 'q') {
    count += semaphores(screen, keyboard, index);

    printf("%d Enter < 80 characters or q to quit:\n", index);
    scanf("%s\n", input);
    printf("%d Input: %s\n", index, input);


  //  	   prompt "enter < 80 characters or q to quit: "
  //  	   read keboard
  //  	   echo
	//    sem_post or give back both semaphores
  }

	// until quit end inner loop in C do ... while not q
	//    count += getSemaphores(index)
	//      prompt "This process had " + count + " deadlocks "
	//    sem_post or give back both semaphores
	// sem_close 
	printf("Process %d finished\n", index);


}

int semaphores (sem_t * screen, sem_t * keyboard, int index) {
  //returns count of how many times recovered from deadlock
  //odd index gets screen first
  //even index gets keyboard first
  int count = 0;
  int s = -1;
  int k = -1;

  // Even
  if(index % 2) {
    k = sem_wait(keyboard);
    printf("%d locked keyboard\n", index);
  }
  // Odd
  else {
    s = sem_wait(screen);
    printf("%d locked screen\n", index);
  }
  	
  // Loop until both semaphores are locked
  do {
    // Even
    if(index % 2) {
      s = sem_timedwait(screen, ts);
      printf("%d locked screen\n", index);
    }
    // Odd
    else {
      k = sem_timedwait(keyboard, ts);
      printf("%d locked keyboard\n", index);
    }

  }
  while(s != 0 || k != 0);
  // 	loop
  // 	sem_timedwait with timer for second
  //  	if timeout 
  //    		give back first, (sem_post)
  //    		wait random time
		// increment count
  //  	else
  //    		have both  true   
 	// 		until have both
 	return count;
} //end get semaphores function














