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
static processList currentList;
static process * executingP;
