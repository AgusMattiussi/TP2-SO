#include <types.h>

#define NAME_MAX_SIZE 25
#define STACK_SIZE 4096

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

static int dummyP(int argc, char **argv)
{
    while (1) {
        _hlt();
    }

    return 0;
}

void initScheduler()
{
    // Se inicializa la lista
    currentList.first = NULL;
    currentList.last = NULL;
    currentList.size = 0;
    currentList.readyCount = 0;
    executingP = NULL;

    // Se agrega el proceso dummy manualmente al inicio
    char *argv[] = {"dummyP"};
    createProcess((void *)&dummyP, 1, argv);
    // dummyProcess = removeProcess(&currentList);
}

// Setea el PCB con los valores nuevos
static uint64_t initProcess(process *pNode, char *name) {
    processContext *pc = &(pNode->pc);
    pc->pid = generatePid();
    memcpy(pc->name, name, strlen(name));
    pc->rbp = (uint64_t)pNode + STACK_SIZE + sizeof(process) - sizeof(char *);
    pc->rsp = (uint64_t)(pc->rbp - sizeof(stackFrame));
    pc->state = READY;
    // pc->priority = INIT_PRIORITY;
    // pc->tickets = INIT_PRIORITY * QUANTUM;
    return pc->pid;
}

static void initStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), uint64_t pid)
{
    stackFrame *stack = (stackFrame *)(pNode->pc.rsp);

    stack->r15 = 0x001;
    stack->r14 = 0x002;
    stack->r13 = 0x003;
    stack->r12 = 0x004;
    stack->r11 = 0x005;
    stack->r10 = 0x006;
    stack->r9 = 0x007;
    stack->r8 = 0x008;

    stack->rsi = (uint64_t)argv;
    stack->rdi = argc;
    stack->rbp = 0;
    stack->rdx = (uint64_t)fn;
    stack->rcx = pid;
    stack->rip = (uint64_t)bootStart;
    stack->cs = 0x8;
    stack->rflags = 0x202;
    stack->rsp = (uint64_t)(pNode->pc.rsp);
    stack->ss = 0x0;
}

static uint64_t generatePid()
{
    return ++lastGivenPid;
}

void bootStart(int argc, char *argv[], void *fn(int, char **))
{
    fn(argc, argv);
    exitProcess();
}

static void exitProcess()
{
    kill(currentProcess->pcb.pid);
}

uint64_t kill(uint64_t pid)
{
    uint16_t done = changeState(pid, KILLED);
    
    if (pid == currentProcess->pcb.pid) {
        forceTimer();
    }

    return done;
}