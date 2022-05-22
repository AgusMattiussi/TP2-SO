#include <types.h>

#define NAME_MAX_SIZE 25
#define PROCESS_STACK_SIZE 0x1000

extern void timerInterrupt();
process * getProcess(uint64_t pid);
int changeProcessState(uint64_t pid, states state);
void exit();
uint64_t getPid();
uint64_t kill(uint64_t pid);
uint64_t block(uint64_t pid);
uint64_t unblock(uint64_t pid);

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

static pid_t lastGivenPid = 1;
static processList *currentList;
static process * executingP;

static int dummyP(int argc, char **argv);
static uint64_t initProcess(process *pNode, char *name);
static void initStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), uint64_t pid);
static uint64_t generatePid();

extern void timerInterrupt();
process * getProcess(uint64_t pid);
int changeProcessState(uint64_t pid, states state);
void exit();
uint64_t getPid();
uint64_t kill(uint64_t pid);
uint64_t block(uint64_t pid);
uint64_t unblock(uint64_t pid);

static void enqProcess(process * pr) {

    /* Si la lista no existe o el proceso es nulo, no puedo encolarlo */
    if(pr == NULL || currentList == NULL)
        return;
    
    /* Si no habia procesos en la lista, este es el primero */
    if(currentList->first == NULL)
        currentList->first = pr;
    /* Si no, es el nuevo ultimo */
    else
        currentList->last->next = pr;
    
    /* Actualizo el puntero al ultimo */
    currentList->last = pr;
    /* Como es lista circular, el siguiente del ultimo es el primero */
    pr->next = currentList->first;

    /* Por default asumimos que el proceso se crea en READY */
    currentList->readyCount++;
    currentList->size++;
}

static process * deqProcess() {

    /* Si la lista no existe o el proceso es nulo, no puedo sacarlo */
    if(currentList == NULL || currentList->size == 0)
        return NULL;

    /* Tomo el primero (asumo FIFO). No puede ser NULL */
    process * deq = currentList->first;

    /* Si habia un solo elemento, entonces first == last. Seteo ambos en
     * NULL, pues ahora la lista quedara vacia */
    if(currentList->size == 1){
        currentList->first = NULL;
        /* Creo que es redundante, porque first==last */
        currentList->last = NULL;
    } else {
        /* Si no, el first sera el siguiente al que voy a sacar */
        currentList->first = deq->next;
        /* Como habia sacado first, el siguiente de last quedo NULL. Lo actualizo */
        currentList->last->next = currentList->first;
    }

    /* Elimino la referencia al siguiente */
    deq->next = NULL;

    /* Si estaba READY, actualizo readyCount*/
    if(deq->pc.state == READY)
        currentList->readyCount--;

    currentList->size--;
    return deq;
}

pid_t createProcess(void (*pFunction)(int, char **), int argc, char **argv){

    //TODO: Cambiar a malloc a secas
    process * new = b_malloc(sizeof(process) + PROCESS_STACK_SIZE);
    if(new == NULL)
        return 0;

    char * prName = argv[0];
    if(initProcess(new, prName) == 0)
        return 0;

    //TODO: Cambiar a malloc a secas
    char **prArgs = b_malloc(sizeof(char *) * argc);
    if(prArgs == NULL)
        return 0;

    //TODO: Implementar copyArguments
    //TODO: Llamar a initializeStackFrame
    enqProcess(new);
    return new->pc.pid;
}

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
    currentList->first = NULL;
    currentList->last = NULL;
    currentList->size = 0;
    currentList->readyCount = 0;
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
    pc->rbp = (uint64_t)pNode + PROCESS_STACK_SIZE + sizeof(process) - sizeof(char *);
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
    exit();
}

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
