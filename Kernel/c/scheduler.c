#include <scheduler.h>

static uint64_t block(pid_t pid);
static uint64_t unblock(pid_t pid);
static int firstProcess(int argc, char **argv);
static pid_t initProcess(process *pNode, char *name);
static void initStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), pid_t pid);
static pid_t generatePid();

static pid_t lastGivenPid = 1;
static processList *currentList;
static process * executingP;

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

static int firstProcess(int argc, char **argv) {
    while (1)
        _hlt();
    
    return 0;
}

void initScheduler() {
    // Se inicializa la lista
    currentList->first = NULL;
    currentList->last = NULL;
    currentList->size = 0;
    currentList->readyCount = 0;
    executingP = NULL;

    // Se agrega el proceso dummy manualmente al inicio
    char *argv[] = {"firstProcess"};
    createProcess((void *)&firstProcess, 1, argv);
    // fp = removeProcess(&currentList);
}

// Setea el PCB con los valores nuevos
static pid_t initProcess(process *pNode, char *name) {
    processContext *pc = &(pNode->pc);
    pc->pid = generatePid();
    memcpy(pc->name, name, strlen(name));
    pc->rbp = (uint64_t)pNode + PROCESS_STACK_SIZE + sizeof(process) - sizeof(char *);
    pc->rsp = (uint64_t)(pc->rbp - sizeof(stackFrame));

    /* Se asume que todos los procesos comienzan en READY */
    pc->state = READY;

    /* IMPLEMENTAR PRIORIDADES */
    return pc->pid;
}

static void initStackFrame(int argc, char **argv, process *pNode, void (*processFn)(int, char **), pid_t pid) {
    stackFrame *stack = (stackFrame *)(pNode->pc.rsp);

    stack->r15 = 0x1;
    stack->r14 = 0x2;
    stack->r13 = 0x3;
    stack->r12 = 0x4;
    stack->r11 = 0x5;
    stack->r10 = 0x6;
    stack->r9 = 0x7;
    stack->r8 = 0x8;

    stack->rsi = (uint64_t)argv;
    stack->rdi = argc;
    stack->rbp = 0;
    stack->rdx = (uint64_t)processFn;
    stack->rcx = pid;
    stack->rip = (uint64_t)forceExitAfterExec;
    stack->cs = 0x8;
    stack->rflags = 0x202;
    stack->rsp = (uint64_t)(pNode->pc.rsp);
    stack->ss = 0x0;
}

static pid_t generatePid() {
    return lastGivenPid++;
}

void forceExitAfterExec(int argc, char *argv[], void *processFn(int, char **)) {
    processFn(argc, argv);
    exitPs();
}

process * getProcess(pid_t pid){
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

int changeProcessState(pid_t pid, states state){
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

void exitPs(){
    kill(executingP->pc.pid);
}

pid_t getPid(){
    return executingP->pc.pid;
}


uint64_t kill(pid_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, KILLED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

uint64_t toggleBlocked(pid_t pid) {
    switch (getProcess(pid)->pc.state) {
        case READY:
            return block(pid);
        case BLOCKED:
            return unblock(pid);
        case KILLED:
            return -1;
    }
}

static uint64_t block(pid_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, BLOCKED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

static uint64_t unblock(pid_t pid){
    if(pid < 1)
        return -1;
    return changeProcessState(pid, READY);
}

// static void freeProcess(process * process){
//     my_free((void *)process)
// }
