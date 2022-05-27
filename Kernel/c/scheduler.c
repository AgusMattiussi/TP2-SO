#include <scheduler.h>

static uint64_t block(pid_t pid);
static uint64_t unblock(pid_t pid);
static int firstProcess(int argc, char **argv);
static pid_t initProcess(process *pNode, char *name);
static void setStackFrame(int argc, char **argv, process *pNode, void (*fn)(int, char **), pid_t pid);
static pid_t generatePid();
static process * getNext(processList * list);
static void enqProcess(processList * list, process * p);
static process * getProcess(processList * list, pid_t pid);
static process * getProcessAndPrevious(processList * list, pid_t pid, process ** prev);
static void setArgs(char ** to, char ** from, int argc);
static void freeProcess(process * p);
static void exitPs();
static processList * initializeProcessList();
static void printProcessListInfo(processList * list);
static void printProcessInfo(process * p);

static pid_t lastGivenPid = 1;
static processList * readyList;
static processList * blockedList;
static process * executingP;

/* Scheduler FIFO */
//TODO: Cambiar a Round Robin con Prioridades
uint64_t scheduler(uint64_t prevRsp){
    //ncPrint("scheduler\n");

    if(readyList == NULL || executingP == NULL)
        return prevRsp;

    executingP->pc.rsp = prevRsp;
    executingP = getNext(readyList);

    //ncPrintWithColor(executingP->pc.name, executingP->pc.name[0] == 'T' ? RED_BLACK : GREEN_BLACK);
    return executingP->pc.rsp;
}

/* Agrega un proceso a la lista */
static void enqProcess(processList * list, process * p) {
    /* Si la lista no existe o el proceso es nulo, no puedo encolarlo */
    if(p == NULL || list == NULL)
        return;
    
    /* Si no habia procesos en la lista, este es el primero */
    if(list->size == 0) {
        list->first = p;
        list->iterator = p;
    /* Si no, es el nuevo ultimo */
    } else {
        list->last->next = p;
    }
    
    /* Actualizo el puntero al ultimo (que ahora es este proceso)*/
    list->last = p;
    /* Como es lista circular, el siguiente del ultimo es el primero */
    p->next = list->first;

    list->size++;
}

/* Elimina un proceso de la lista y lo retorna */
static process * delProcess(processList * list, pid_t pid) {

    /* Si la lista no existe o el proceso es nulo, no puedo sacarlo */
    if(list == NULL || list->size == 0)
        return NULL;

    process * prev = NULL;
    process * toDel = getProcessAndPrevious(list, pid, &prev);
    
    if(toDel == NULL)
        return NULL;
    
    /* Si habia un solo elemento, entonces first == last == iterator. Seteo todos en
     * NULL, pues ahora la lista quedara vacia */
    if(list->size == 1){
        list->first = NULL;
        /* FIXME: Creo que es redundante, porque first == last == iterator */
        list->last = NULL;
        list->iterator = NULL;
    } else {
        /* Si justo estoy por borrar el nodo al que apunta iterator, entonces hago que
         * este utlimo avance */
        if(toDel == list->iterator)
            list->iterator = list->iterator->next;
        /* Salteo a toDel en la lista */
        prev->next = toDel->next;
    }

    /* Elimino la referencia al siguiente */
    toDel->next = NULL;
    /* Actualizo el tamaño de la lista */
    list->size--;
    return toDel;
}

static process * getProcess(processList * list, pid_t pid){
    process * toDiscard = NULL;
    return getProcessAndPrevious(list, pid, &toDiscard);
}

/* Obtiene un proceso especifico de la lista por PID, pero no lo elimina de la misma.
 * Adicionalmente, en el puntero *previous devuelve el nodo anterior al retornado */
static process * getProcessAndPrevious(processList * list, pid_t pid, process ** prev){
    /* Si la lista no existe, esta vacia o el PID es invalido, retorno */
    if(list == NULL || list->size == 0 || pid <= 0)
        return NULL;

    /* Tomo el primero. En este punto, no puede ser NULL */
    process * toRet = list->first;
    *prev = toRet;
    /* Recorro la lista buscando aquel proceso que coincida en PID */
    int i;
    for (i = 0; i < list->size; i++){
        if(toRet->pc.pid == pid)
            break;
        *prev = toRet;
        toRet = toRet->next;
    }

    /* Si se recorrio toda la lista, significa que no se encontro */
    if(i == list->size){
        *prev = NULL;
        return NULL;
    }

    return toRet;
}

/* Devuelve el proximo proceso listo de la lista y actualiza el iterador */
static process * getNext(processList * list) {
    if(list == NULL || list->size == 0)
        return NULL;
    
    process * next = list->iterator;
    
    list->iterator = list->iterator->next;
    return next;
}

/* Crea un nuevo proceso y lo agrega a la lista de procesos READY. Retorna el nuevo PID */
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
    enqProcess(readyList, new);
    return new->pc.pid;
}

/* Libera el proceso p y todos sus recursos */
static void freeProcess(process * p){
    //TODO: Chequear si esto esta bien
    /* Obtengo argc y argv desde el stack del proceso */
    //int argc = *((uint64_t *)(p->pc.rbp) - 11 * sizeof(uint64_t)); // rdi
    //char ** argv = *((char ***)((uint64_t *)(p->pc.rbp) - 12 * sizeof(uint64_t))); // rsi

    /* Libero los argumentos */
    /* for (int i = 0; i < argc; i++){
        ncPrintWithColor(argv[i], CYAN_BLACK);
        free(argv[i]);}
    free(argv); */
    
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
 * las listas de procesos y crea el primer proceso */
void initScheduler() {
    //ncPrintWithColor("INICIALIZANDO SCHEDULER\n", 0x02);

     /* Se inicializa la lista circular de READY */
    readyList = initializeProcessList();
    if(readyList == NULL)
        return;
    /* Se inicializa la lista circular de BLOCKED */
    blockedList = initializeProcessList();
    if(blockedList == NULL)
        return;
    /* Se agrega el primer proceso manualmente */
    createFirstProcess();

    //TODO: Chequear si no hay que sacarlo
    executingP = readyList->first;
    //ncPrintWithColor(executingP->pc.name, 0x02);
    // TODO: deqProcess();
    // free(deqProcess());
}

/* Inicializa una lista de procesos. Devuelve 0 en caso de error o
 * 1 en caso contrario */
static processList * initializeProcessList() {
    processList * list = malloc(sizeof(processList));
    if(list == NULL)
        return NULL;
    list->first = NULL;
    list->last = NULL;
    list->iterator = NULL;
    list->size = 0;

    return list;
}

/* Crea el primer proceso y le asigna su nombre */
void createFirstProcess(){
    char *argv[] = {"firstProcess"};
    createProcess((void *)&firstProcess, 1, argv);
}

/* Primer proceso creado. Su unica funcion es esperar a que llegue un
 * proceso real */
static int firstProcess(int argc, char **argv) {
    while (1)
        _hlt();

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

/* Se liberan los recursos de un proceso que haya terminado su ejecucion */
static void exitPs(){
    kill(executingP->pc.pid);
}

/* Devuelve el PID del proceso en ejecucion */
//TODO: que pasa si executingP es NULL?
pid_t getPid(){
    return executingP->pc.pid;
}

/* Se mata un proceso segun su PID, eliminando sus recursos. Devuelve 1 si fue
 * exitoso o 0 en caso de error */
uint64_t kill(pid_t pid){
    int isExecuting = pid == executingP->pc.pid;

    /* Buscamos el proceso a eliminar en ambas listas */
    process * toKill;
    toKill = delProcess(readyList, pid);
    if(toKill == NULL)
        toKill = delProcess(blockedList, pid);
    if(toKill == NULL)
        return 0;

    freeProcess(toKill);

    // TODO: Hace falta?
    if(isExecuting)
        timerInterrupt();

    return 1;
}

/* Alterna el estado de un proceso entre READY y BLOCKED */
uint64_t toggleBlocked(pid_t pid) {
    int aux = block(pid);
    if(aux == 0)
        aux = unblock(pid);
    return aux;
}

/* Cambia el estado de un proceso a BLOCKED. Devuelve 1 si fue
 * exitoso o 0 en caso de error */
static uint64_t block(pid_t pid){
    process * p = delProcess(readyList, pid);
    if(p == NULL)
        return 0;
    
    p->pc.state = BLOCKED;
    enqProcess(blockedList, p);

    //TODO: Hace falta?
    if(pid == executingP->pc.pid)
        timerInterrupt();

    return 1;
}

/* Cambia el estado de un proceso BLOCKED a READY */
static uint64_t unblock(pid_t pid){
    process * p = delProcess(blockedList, pid);
    if(p == NULL)
        return 0;
    p->pc.state = READY;
    enqProcess(readyList, p);
    return 1;
}

/* Imprime cada proceso junto con su informacion */
void printAllProcessesInfo(){
    // ncPrint("Lista de procesos\n");
    if(readyList->size == 0 && blockedList->size == 0){
        ncPrint("No hay ningun proceso ejecutandose");
        return;
    }
    
    //TODO: Falta prioridad
    ncPrint("PID    NAME            RSP    RBP   STATE\n");
    printProcessListInfo(readyList);
    printProcessListInfo(blockedList);
}

static void printProcessListInfo(processList * list) {
    process * toPrint;
    if(list->size > 0){
        toPrint = list->first;
        for (int i = 0; i < list->size; i++){
            printProcessInfo(toPrint);
            toPrint = toPrint->next;
        }
    }
}

static void printProcessInfo(process * p){
    ncPrintDec(p->pc.pid);
    ncPrint(TAB);

    ncPrint(p->pc.name);
    ncPrint(TAB);

    ncPrintHex(p->pc.rsp);
    ncPrint(TAB);

    ncPrintHex(p->pc.rbp);
    ncPrint(TAB);

    switch(p->pc.state) {
        case READY: 
            ncPrint("READY");
            break;
        case BLOCKED:
            ncPrint("BLOCKED");
            break;
        default:
            ncPrint("?????");
    }

    ncPrint("\n");
}