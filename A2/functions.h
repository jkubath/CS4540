#ifndef functions_h
#define functions_h
void readData(process * queueIn);
void addProcess(process * p, ui * queue, int index, ui * queueCount);
void printQueue(ui * queue, ui * queueCount);
int run(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount, int swap, ui * totalProcess);
void swapToIo(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount);
void incrementIo(process * p, ui * queue, ui * queueCount, ui * io, ui * ioCount, ui * totalProcess);
void incrementWait(process * p, ui * queue, ui * queueCount);
void addWaitData(process * p, ui * queue);

#endif
