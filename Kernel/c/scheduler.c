#include <types.h>

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
static processList * currentList;
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
    char **prArgs = b_malloc(sizeof(char *) * argc)
    if(prArgs == NULL)
        return 0;

    //TODO: Implementar copyArguments
    //TODO: Llamar a initializeStackFrame
    enqProcess(new);
    return new->pc.pid;
}