#ifndef __SEMAPHORES_H_
#define _SEMAPHORES_H_

#include <stdint.h>
#include <stddef.h>
#include <scheduler.h>
#include <memory.h>
#include <memoryManager.h>
#include <strings.h>
#include <naiveConsole.h>

#define NAME_MAX_SIZE 25
#define FAILED 0
#define SUCCESS 1

typedef struct pList{
    struct pList * next;
    pid_t pid;
} pList;

typedef struct TSem {
    //TODO: Revisar si esta bien que un semaforo tenga un puntero al siguiente
    struct TSem * next;
    char name[NAME_MAX_SIZE];
    uint64_t lock;
    int value;
    // lista de procesos que tiene en un wait
    pList * firstProcess;
    pList * lastProcess;
    int waitingProcesses;
} TSem;

typedef struct semList{
    TSem * first;
    TSem * last;
    int size;
} semList;

extern uint64_t _xchgLock(uint64_t * lock);
extern uint64_t _unlock(uint64_t * lock);

uint64_t semOpen(char *name, int initialValue);

uint64_t semClose(TSem * sem);

uint64_t semWait(TSem * sem);

uint64_t semPost(TSem * sem);

void printListofSemaphores();

#endif