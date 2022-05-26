#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <types.h>
#include <memory.h>
#include <memoryManager.h>
#include <strings.h>
#include <naiveConsole.h>

#define NAME_MAX_SIZE 25
#define PROCESS_STACK_SIZE 0x1000

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
    pid_t pid;
    uint64_t rsp;
    uint64_t rbp;
    /* Priodad del proceso ? */
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

extern void timerInterrupt();
extern void _hlt();
int changeProcessState(pid_t pid, states state);
void forceExitAfterExec(int argc, char *argv[], void *processFn(int, char **));
pid_t getPid();
uint64_t kill(pid_t pid);
uint64_t toggleBlocked(pid_t pid);
uint64_t block(pid_t pid);
void printListOfProcesses();
void initScheduler();
uint64_t scheduler(uint64_t prevRsp);
void createFirstProcess();
pid_t createProcess(void (*pFunction)(int, char **), int argc, char **argv);


#endif