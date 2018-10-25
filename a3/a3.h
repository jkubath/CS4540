#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void Process (sem_t * screen, sem_t * keyboard, int index);
int semaphores (sem_t * screen, sem_t * keyboard, int index);
