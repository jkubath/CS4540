/*
	Name: Jonah Kubath
	Class: CS4540
	Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include "a2.h"
#include "functions.h"


int main(int argc, char ** argv) {
	process p[48]; // process id is array index

	// variables below hold index to process in this array (process id)
    ui queue[48]; //Process queue, index value is process id
    ui io[48]; //Process in I/O
    ui queueCount = 0;
    ui ioCount = 0;
    ui totalProcess = 0;

    os * os_data = calloc(sizeof(os), 1);
    os_data->quantum = 50; //Number of clock ticks before a process is preempted
    os_data->wait = 50; //Number of clock ticks before priority is increased due to starvation

    readData(p); // Read the process data

	int count = 1; //Current clock tick
    int quantum = 1; //Current time quantum
    int swap = 0; //Force current process to swap 1 = force, 0 = stays
    do {
   		/* Adding processes to queue */
    	if(count <= 48) {
    		addProcess(p, queue, count - 1, &queueCount);
    		totalProcess++;
    	}
    	/* Done adding processes to queue */

    	//Time quantum reached - swap process
    	if(quantum == os_data->quantum) {
    		swap = run(p, queue, &queueCount, io, &ioCount, 1, &totalProcess);
   			swap = 1;
    	}
    	else
    		swap = run(p, queue, &queueCount, io, &ioCount, 0, &totalProcess);

    	//If the process is swapped out, reset time quantum
    	if(swap == 1) {
    		quantum = 0;
    	}
    	else
    	{
    		quantum++;
    	}

    	count++;
    } while(totalProcess > 0);

    //Print wait data
   	printStats(p, *os_data);
	return 0;
}

void readData(process * processIn) {
	process * p = processIn;
	int index = 0;
	FILE* ptr = fopen("a2in.txt","r");
	int read = -1;

	while(index < 48 && read != 0) 
	{
		
		read = fscanf(ptr, "%hu %hu %hu %hu", &(p[index].priority), &(p[index].cpu), &(p[index].io), &(p[index].runTime));
		//Initialize other data
		p[index].curCpu = 0;
		p[index].curIo = 0;
		p[index].wait = 0;
		p[index].curCpu = 0;
		p[index].curPrior = p[index].priority;
		p[index].cpuTotal = 0;
		p[index].ioTotal = 0;
		p[index].waitSum = 0;
		p[index].waitCount = 0;
		p[index].waitMin = 0;
		p[index].waitMax = 0;

		index++;
		
	}	
}

void addProcess(process * p, ui * queue, int index, ui * queueCount) {
	int cur = 1;
	//If no processes in queue, add to front
	if(*queueCount == 0) {
		cur = 0;
	}
	else {
		//Find the spot to add the process 15 (highest) - 0 (lowest) priority
		while(cur < *queueCount && p[index].curPrior <= p[queue[cur]].curPrior) {
			cur++;
		}

		int i = 46;
		while(i >= cur) {
			//Shift all the processes one to the right
			queue[i + 1] = queue[i];
			i--;
		}
	}

	queue[cur] = index;

	(*queueCount)++;
}

void printQueue(ui * queue, ui * queueCount) {
	int index = 0;

	printf("Process Queue %hu\n", *queueCount); 
	while(index < *queueCount) {
		printf("%hu ", queue[index]);
		index++;
	}
	printf("\n");
}

int run(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount, int swap, ui * totalProcess) {
	ui index = queue[0];

	//Only increment run time and wait time if processes are in the queues
	if(*queueCount > 0) {
		p[index].curCpu++;
	}
	if(*queueCount > 1) {
		incrementWait(p, queue, queueCount);
	}

	//Only increment the io processes if there are processes in io
	if(*ioCount > 0) {
		incrementIo(p, queue, queueCount, io, ioCount, totalProcess);
	}

	//Has the current process hit its cpu time
	if(*queueCount > 0) {
		//process should move to io
		if(p[index].curCpu == p[index].cpu) {
			p[index].cpuTotal += p[index].curCpu;
			p[index].curCpu = 0;

			//Only add to io if the process isn't done running
			if((p[index].cpuTotal + p[index].ioTotal) != p[index].runTime){
				swapToIo(p, queue, queueCount, io, ioCount);
			}
			else{
				(*totalProcess)--;
			}

			//Shift the processes forward
			int i =  0;
			while(i < *queueCount - 1) {
				queue[i] = queue[i + 1];
				i++;
			}

			(*queueCount)--; // Remove the process

			if(*queueCount != 0)
				addWaitData(p, queue); //Add wait data to new running process
			swap = 1;
		} //End of swapping process to io
		else if(swap == 1) {
			//Reset the priority
			p[index].curPrior = p[index].priority;
			//Shift everything left one
			int i = 0;
			while(i < *queueCount - 1) {
				queue[i] = queue[i + 1];
				i++;
			}
			(*queueCount)--;

			if(*queueCount != 0)
				addWaitData(p, queue); //Add wait data to new running process

			addProcess(p, queue, index, queueCount);
		}
		
	}
	return swap; // 0 if the process stayed in the cpu, 1 if the process swapped out
}

void swapToIo(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount) {
	ui index = queue[0];

	//Reset io timer
	p[index].curIo = 0;
	io[*ioCount] = index; //Add to io queue

	(*ioCount)++;
}

void incrementIo(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount, ui * totalProcess) {
	int index = 0;
	int temp = 0;

	while(*ioCount != 0 && index < *ioCount) {
		temp = io[index]; //Get current process id

		p[temp].curIo++;
		
		//Process is done in running
		if((p[temp].cpuTotal + p[temp].ioTotal + p[temp].curIo) == p[temp].runTime) {
			
			p[temp].ioTotal += p[temp].curIo;
			p[temp].curIo = 0;

			int i = index;
			//Shift everything left one in the ioQueue
			while(i < *ioCount - 1) {
				io[i] = io[i + 1];
				i++;
			}
			//Set count variables
			(*ioCount)--;
			index--; //Everything was shifted left, so subtract one from current pointer
			(*totalProcess)--;

		}
		//Time to go back to the ready queue
		else if(p[temp].curIo == p[temp].io) {

			p[temp].ioTotal += p[temp].curIo;
			p[temp].curIo = 0;

			addProcess(p, queue, temp, queueCount);

			//Shift all processes forward
			int cur = index;
			while(cur < *ioCount - 1) {
				io[cur] = io[cur + 1];
				cur++;
			}

			//Set count variables
			(*ioCount)--;
			index--; //Everything was shifted left, so subtract one from current pointer
		} //Done moving to ready queue

		index++;
	}
}

void incrementWait(process * p, ui * queue, ui * queueCount){
	int index = 1;
	ui cur = 0;

	if(*queueCount != 0) {
		while(index < *queueCount) {
			cur = queue[index]; //Get process id

			p[cur].wait++;

			//Adjust priority
			if(p[cur].wait % 50 == 0) {
				p[cur].curPrior++;
				//Error checking
				if(p[cur].curPrior > 15) {
					p[cur].curPrior = 15;
				}
			}

			index++;
		}
	}
}

void addWaitData(process * p, ui * queue) {
	/* Set wait data for the new running process */
	ui index = queue[0]; //Get the current index
	//Reset priority of new running process
	p[index].curPrior = p[index].priority;

	//Set waitMin
	if(p[index].waitMin <= 0 || p[index].wait < p[index].waitMin)
		p[index].waitMin = p[index].wait;
	//Set waitMax
	if(p[index].waitMax == 0 || p[index].wait > p[index].waitMax)
		p[index].waitMax = p[index].wait;
	//Set waitSum
	p[index].waitSum += p[index].wait;
	p[index].wait = 0;

	//Process is done waiting
	p[index].waitCount++;
}
