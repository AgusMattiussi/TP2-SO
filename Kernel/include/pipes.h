#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>
#include <stddef.h>
#include <scheduler.h>
#include <memory.h>
#include <memoryManager.h>
#include <strings.h>
#include <semaphores.h>

#define BUFFER_SIZE 1024
#define PIPE_NAME_SIZE 25

#define READ_SEM_NAME "readSem"
#define WRITE_SEM_NAME "writeSem"

typedef struct TPipe{
    struct TPipe * next;
    int fds[2];
    char name[PIPE_NAME_SIZE];
    char buffer[BUFFER_SIZE];
    uint64_t readIndex;
    uint64_t writeIndex;
    char readSemName[NAME_MAX_SIZE]; 
    char writeSemName[NAME_MAX_SIZE]; 
    int numOfProcessesAttached; // cantidad de procesos que estan usando el pipe
} TPipe;

typedef struct pipeList{
    TPipe * first;
    TPipe * last;
    int size;
} pipeList;

void initPipes();
int * pipeOpen(char *name);
uint64_t pipeClose(char * pipeName);
uint64_t writePipe(char * pipeName, char *str);
char readPipe(char * pipeName);
void printListOfPipes();

uint64_t writeCharInPipe(TPipe * pipe, char c);

#endif