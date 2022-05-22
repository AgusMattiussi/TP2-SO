#include <types.h>

#define NAME_MAX_SIZE 25

typedef enum {READY, BLOCKED, KILLED} states;

typedef struct stackFrame{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;   // argv
    uint64_t rdi;   // argc
    uint64_t rbp;
    uint64_t rdx;   // function
    uint64_t rcx;   // pid
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;    // loader
    uint64_t cs;     // 0x8
    uint64_t rflags; // 0x202
    uint64_t rsp;    // rbp
    uint64_t ss;     // 0
} stackFrame;

typedef struct processContext{
    char name[NAME_MAX_SIZE];
    uint64_t pid;
    uint64_t rsp;
    uint64_t rbp;
    // uint64_t priority;
    // uint64_t tickets;
    states state;
} processContext;

typedef struct process{
    struct process * next;
    processContext pc;
} process;

typedef struct processList{
    process * first;
    process * last;
    int size;
    int readyCount;
} processList;

static uint64_t lastGivenPid = 1;
static processList * currentList;
static process * executingP;

extern void timerInterrupt();
process * getProcess(uint64_t pid);
int changeProcessState(uint64_t pid, states state);
void exit();
uint64_t getPid();
uint64_t kill(uint64_t pid);
uint64_t block(uint64_t pid);
uint64_t unblock(uint64_t pid);

process * getProcess(uint64_t pid){
    if(pid == executingP->pc.pid)
        return executingP;

    process * aux = currentList->first;
    int i=0;

    while(i < currentList->size){
        if(pid == aux->pc.pid)
            return aux;
        aux = aux->next;
        i++;
    }

    return NULL;
}

int changeProcessState(uint64_t pid, states state){
    if(pid == executingP->pc.pid){
        executingP->pc.state = state;
        return 0;
    }

    process * aux = getProcess(pid);

    if(aux == NULL || aux->pc.state == KILLED)
        return -1;
    
    if(aux->pc.state != READY && state == READY)
        currentList->readyCount++;
    
    if(aux->pc.state == READY && state != READY)
        currentList->readyCount--;

    aux->pc.state = state;
    return 0;
}

void exit(){
    kill(executingP->pc.pid);
}

uint64_t getPid(){
    return executingP->pc.pid;
}


uint64_t kill(uint64_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, KILLED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

uint64_t block(uint64_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, BLOCKED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

uint64_t unblock(uint64_t pid){
    if(pid < 1)
        return -1;
    return changeProcessState(pid, READY);
}

// static void freeProcess(process * process){
//     my_free((void *)process)
// }