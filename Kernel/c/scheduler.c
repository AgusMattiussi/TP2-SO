#include <scheduler.h>

static uint64_t unblock(pid_t pid);
static int firstProcess(int argc, char **argv);
static pid_t initProcess(process *pNode, char *name);
static void setStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), pid_t pid);
static pid_t generatePid();
static process * getReadyPs();
static void enqProcess(process * pr);
static process * deqProcess();
static process * getProcess(pid_t pid);
static void setArgs(char ** to, char ** from, int argc);
static void freeProcess(process * p);
static void exitPs();

static pid_t lastGivenPid = 1;
static processList * currentList;
static process * executingP;

/* Scheduler FIFO */
//TODO: Cambiar a Round Robin con Prioridades
uint64_t scheduler(uint64_t prevRsp){

    //ncPrint("\nscheduler");

    if(currentList == NULL)
        return prevRsp;

    /* for (int i = 0; i < currentList->size; i++){
        ncPrint(currentList->last->pc.name);
    }
     */

    if(executingP == NULL){
        //ncPrint("penis\n");
        return prevRsp;}

    process * nextPs = getReadyPs();
    if(nextPs == NULL)
        return prevRsp;

    executingP->pc.rsp = prevRsp;
    enqProcess(executingP);
    executingP = nextPs;

    
    return executingP->pc.rsp;
}

/* Agrega un proceso a la lista */
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

    //ncPrintWithColor(currentList->last->pc.name, 0x02);
}

/* Elimina y devuelve el primer proceso de la lista */
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

    /* Actualizo el tamaño de la lista */
    currentList->size--;
    return deq;
}

/* Obtiene un proceso especifico de la lista, pero no lo elimina de la misma */
static process * getProcess(pid_t pid){
    /* Si justo es el proceso en ejecucion, entonces no esta en la lista */
    if(executingP != NULL && pid == executingP->pc.pid)
        return executingP;

    /* Itero por la lista de procesos buscando el que me piden por parametro */
    process * aux = currentList->first;
    for(int i = 0; i < currentList->size; i++) {
        if(pid == aux->pc.pid)
            return aux;
        aux = aux->next;
    }

    /* Si llegue a este punto, no se encontro el proceso */
    return NULL;
}

/* Obtiene el proximo proceso READY de la lista y elimina cualquier proceso
 * KILLED que encuentre en el recorrido */
static process * getReadyPs() {
    process * currentPs;
    size_t psCount = currentList->size;

    for (size_t i = 0; i < psCount; i++){
        /* Tomo el proximo proceso */
        currentPs = deqProcess();

        /* Si es nulo, devuelvo NULL. Si esta READY, lo devuelvo. */
        if(currentPs == NULL || currentPs->pc.state == READY)
            return currentPs;

        /* Si esta KILLED, lo libero */
        if(currentPs->pc.state == KILLED){
            freeProcess(currentPs);
        } else {
        /* En caso contrario, esta BLOCKED, por lo que lo devuelvo a la lista */
            enqProcess(currentPs);
        }
    }
    
    /* Si llego hasta este punto, no se encontraron procesos READY */
    return NULL;
}

/* Crea un nuevo proceso y lo agrega a la lista de procesos. Retorna el nuevo PID */
pid_t createProcess(void (*pFunction)(int, char **), int argc, char **argv){
    
    /* Reservo espacio para el nuevo nodo de proceso. Notemos que new incluye
     * al proceso y al stack del mismo */ 
    //TODO: Chequear si esto esta bien
    process * new = malloc(sizeof(process) + PROCESS_STACK_SIZE);
    if(new == NULL)
        return 0;

    /* El primer parametro de argv es el nombre del proceso. Lo guardo en
     * prName e inicializo el proceso new con dicho nombre */
    char * prName = argv[0];
    if(initProcess(new, prName) == 0)
        return 0;
    

    /* Reservamos espacio para 'argc' argumentos */
    char **prArgs = malloc(sizeof(char *) * argc);
    if(prArgs == NULL)
        return 0;

    /* Copiamos uno por uno los argumentos recibidos como parametro a prArgs */
    setArgs(prArgs, argv, argc);
    /* Creamos un stack frame para el proceso, simulando que 'siempre existio' */
    setStackFrame(argc, prArgs, new, pFunction, new->pc.pid);

    /* Se agrega el nuevo proceso a la lista*/
    enqProcess(new);
    return new->pc.pid;
}

/* Libera el proceso p y todos sus recursos */
static void freeProcess(process * p){
    //TODO: Chequear si esto esta bien
    /* Obtengo argc y argv desde el stack del proceso */
    int argc = *((uint64_t *)(p->pc.rbp) + 9 * sizeof(uint64_t)); // rdi
    char ** argv = *((char ***)((uint64_t *)(p->pc.rbp) + 8 * sizeof(uint64_t))); // rsi

    /* Libero los argumentos */
    for (int i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);
    
    /* Libero el nodo del proceso */
    free(p);
}

/* Reserva espacio y asigna los argumentos de un proceso nuevo */
static void setArgs(char ** to, char ** from, int argc){
    int argLen;

    for (int i = 0; i < argc; i++){
        argLen = strlen(from[i]);

        /* Reservo espacio para el argumento i */
        to[i] = malloc(argLen + 1);
        if(to[i] == NULL)
            return;

        /* Copio el argumento i al nuevo arreglo */
        memcpy(to[i], from[i], argLen);
    }  
}

/* Se llama a esta funcion antes de comenzar a operar con el scheduler. Inicializa
 * la lista de procesos y crea el primer proceso */
void initScheduler() {
    ncPrintWithColor("INICIALIZANDO SCHEDULER\n", 0x02);
    /* Se inicializa la lista circular */
    currentList = malloc(sizeof(processList));
    if(currentList == NULL)
        return;
    currentList->first = NULL;
    currentList->last = NULL;
    currentList->size = 0;
    currentList->readyCount = 0;
    executingP = NULL;

    /* Se agrega el primer proceso manualmente */
    createFirstProcess();
    executingP = deqProcess();
    //ncPrintWithColor(executingP->pc.name, 0x02);
    // TODO: deqProcess();
    // free(deqProcess());
}

/* Crea el primer proceso y le asigna su nombre */
void createFirstProcess(){
    char *argv[] = {"firstProcess"};
    createProcess((void *)&firstProcess, 1, argv);
}

/* Primer proceso creado. Su unica funcion es esperar a que llegue un
 * proceso real */
static int firstProcess(int argc, char **argv) {
    //ncPrintWithColor("Primer Proceso\n", 0x02);
    
    while (1){
        //ncPrintWithColor("Halt", 0x02);
        _hlt();
    }
    
    return 0;
}

/* Inicializa Process Context de un nuevo proceso con los valores
 * correspondientes */
static pid_t initProcess(process *pNode, char *name) {
    processContext *pc = &(pNode->pc);
    
    /* Genero un nuevo PID para el proceso */
    pc->pid = generatePid();
    /* Copio el nombre recibido por parametro al campo name de pc */
    memcpy(pc->name, name, strlen(name));
    
    /* rbp apunta a la base del stack */
    //TODO: Es necesario restar sizeof(char *)?
    pc->rbp = (uint64_t)pNode + PROCESS_STACK_SIZE + sizeof(process) - sizeof(char *);
    /* Stack pointer apunta a la primera direccion libre del stack: despues del
     * stackFrame */
    pc->rsp = (uint64_t)(pc->rbp - sizeof(stackFrame));

    /* Se asume que todos los procesos comienzan en READY */
    pc->state = READY;

    //TODO: IMPLEMENTAR PRIORIDADES
    return pc->pid;
}

/* Se inicializa el Stack Frame de un nuevo proceso. En los registros r8 a r9, se asignan
 * numeros consecutivos con el objetivo de facilitar el debugging. El significado
 * de lo que se asigna en el resto de los registros, se explica en scheduler.h */
static void setStackFrame(int argc, char **argv, process *pNode, void (*processFn)(int, char **), pid_t pid) {
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
    // stack->rip = (uint64_t)processFn;
    stack->cs = 0x8;
    stack->rflags = 0x202;
    stack->rsp = (uint64_t)(pNode->pc.rsp);
    stack->ss = 0x0;
}

/* Genera un nuevo PID consecutivo al ultimo generado y actualiza lastGivenPid */
static pid_t generatePid() {
    return lastGivenPid++;
}

/* Le agrega a la funcion principal del proceso, una llamada a exitPs para que pase
 * su estado a KILLED despues de ejecutarse */
void forceExitAfterExec(int argc, char *argv[], void *processFn(int, char **)) {
    processFn(argc, argv);
    exitPs();
}

/* Cambia el estado de un proceso cuyo PID se envia por parametro. Devuelve 0 si fue
 * exitoso o -1 en caso de error */
int changeProcessState(pid_t pid, states state){
    /* Busco el proceso indicado por pid */
    process * aux = getProcess(pid);

    /* Si no se encontro el proceso o su estado es KILLED, no puedo cambiarle
     * el estado */
    if(aux == NULL || aux->pc.state == KILLED)
        return -1;
    
    /* Si el proceso no estaba READY y ahora se lo quiere cambiar a READY,
     * actualizo el readyCount */
    if(aux->pc.state != READY && state == READY)
        currentList->readyCount++;
    
    /* Si el proceso estaba READY y ahora se lo quiere cambiar a otro estado,
     * actualizo el readyCount */
    if(aux->pc.state == READY && state != READY)
        currentList->readyCount--;

    /* Actualizo el estado del proceso */
    aux->pc.state = state;
    return 0;
}

/* Se cambia el estado a KILLED de un proceso que haya terminado su ejecucion */
static void exitPs(){
    kill(executingP->pc.pid);
}

/* Devuelve el PID del proceso en ejecucion */
//TODO: que pasa si executingP es NULL?
pid_t getPid(){
    return executingP->pc.pid;
}

/* Cambia el estado de un proceso a KILLED. Devuelve 0 si fue
 * exitoso o -1 en caso de error */
uint64_t kill(pid_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, KILLED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

/* Alterna el estado de un proceso entre READY y BLOCKED */
uint64_t toggleBlocked(pid_t pid) {
    switch (getProcess(pid)->pc.state) {
        case READY:
            return block(pid);
        case BLOCKED:
            return unblock(pid);
        case KILLED:
            return -1;
        default:
            return -1;
    }
}

/* Cambia el estado de un proceso a BLOCKED. Devuelve 0 si fue
 * exitoso o -1 en caso de error */
uint64_t block(pid_t pid){
    if(pid < 1)
        return -1;

    int result = changeProcessState(pid, BLOCKED);

    if(pid == executingP->pc.pid)
        timerInterrupt();

    return result;
}

/* Cambia el estado de un proceso BLOCKED a READY */
static uint64_t unblock(pid_t pid){
    if(pid < 1)
        return -1;
    return changeProcessState(pid, READY);
}

/* Imprime cada proceso junto con su informacion */
void printListOfProcesses(){
    // ncPrint("Lista de procesos\n");
    process * toPrint = currentList->first;
    if(toPrint == NULL){
        ncPrint("No processes to show\n");
        return;
    }
    //TODO: Falta prioridad
    ncPrintWithColor("PID    NAME           RSP       RBP      STATE\n", ORANGE_BLACK);
    for (int i = 0; i < currentList->size; i++){
        ncPrintDec(toPrint->pc.pid);
        ncPrint("    ");
        ncPrint(toPrint->pc.name);
        ncPrint("    ");
        ncPrintHex(toPrint->pc.rsp);
        ncPrint("    ");
        ncPrintHex(toPrint->pc.rbp);
        ncPrint("    ");


        switch(toPrint->pc.state) {
            case READY: 
                ncPrint("READY");
                break;
            case BLOCKED:
                ncPrint("BLOCKED");
                break;
            case KILLED:
                ncPrint("KILLED");
                break;
            default:
                ncPrint("?????");
        }


        ncPrint("\n");

        toPrint = toPrint->next;
        i++;
    }
    toPrint = executingP;
    ncPrintDec(toPrint->pc.pid);
    ncPrint("    ");
    ncPrint(toPrint->pc.name);
    ncPrint("        ");
    ncPrintHex(toPrint->pc.rsp);
    ncPrint("    ");
    ncPrintHex(toPrint->pc.rbp);
    ncPrint("    ");
    
    switch(toPrint->pc.state) {
            case READY: 
                ncPrint("READY");
                break;
            case BLOCKED:
                ncPrint("BLOCKED");
                break;
            case KILLED:
                ncPrint("KILLED");
                break;
            default:
                ncPrint("?????");
        }


    ncPrint("\n");
}