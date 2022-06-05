#include <semaphores.h>

static int createSemaphore(char *name, int initialValue);
static void enqSem(TSem * sem);
static TSem * deqSem();
static void enqPr(TSem * sem, pid_t pid);
static pid_t deqPr(TSem * sem);
static TSem * getSem(char * name);
// static int semExists(char * name);
static void freeSem(TSem * sem);

static uint64_t semLock = 0;
static semList * semaphoresList;

void initSemaphores(){
    semaphoresList = malloc(sizeof(semList));
    semaphoresList->first = NULL;
    semaphoresList->last = NULL;
    semaphoresList->size = 0;
}

static int createSemaphore(char *name, int initialValue){
    TSem * new = malloc(sizeof(TSem));
    if(new == NULL)
        return FAILED;

    //memset(new->name, 0, NAME_MAX_SIZE);
    memcpy(new->name, name, strlen(name)+1);

    new->value = initialValue;
    new->lock = 0;
    new->firstProcess = NULL;
    new->lastProcess = NULL;
    new->waitingProcesses = 0;
    new->openedBy = 1;

    enqSem(new);
    return SUCCESS;
}


static void enqSem(TSem * sem) {
    if(sem == NULL || semaphoresList == NULL)
        return;
    
    if(semaphoresList->first == NULL)
        semaphoresList->first = sem;
    else
        semaphoresList->last->next = sem;
    
    semaphoresList->last = sem;
    //sem->next = semaphoresList->first;
    sem->next = NULL;

    semaphoresList->size++;
}

static TSem * deqSem() {

    if(semaphoresList == NULL || semaphoresList->size == 0)
        return NULL;

    TSem * deq = semaphoresList->first;

    if(semaphoresList->size == 1){
        semaphoresList->first = NULL;
        semaphoresList->last = NULL;
    } else {
        semaphoresList->first = deq->next;
        //semaphoresList->last->next = semaphoresList->first;
    }

    deq->next = NULL;

    semaphoresList->size--;
    return deq;
}

static void enqPr(TSem * sem, pid_t pid){
    
    pNode * process = malloc(sizeof(pNode));

    if(process == NULL)
        return;

    process->pid = pid;

    if(sem->firstProcess == NULL)
        sem->firstProcess = process;
    else
        sem->lastProcess->next = process;

    sem->lastProcess = process;
    process->next = NULL;

    sem->waitingProcesses++;
}

static pid_t deqPr(TSem * sem){
    if(sem->waitingProcesses == 0)
        return FAILED;

    pNode * process = sem->firstProcess;
    int pid = process->pid;

    if(sem->waitingProcesses == 1){
        sem->firstProcess = NULL;
        sem->lastProcess = NULL;
    } else {
        sem->firstProcess = process->next;
        //sem->lastProcess->next = sem->firstProcess;
    }

    process->next = NULL;
    free(process);
    sem->waitingProcesses--;
    return pid;
}

static TSem * getSem(char * name){
    TSem * aux = semaphoresList->first;

    for(int i = 0; i < semaphoresList->size; i++){
        /* ncPrintWithColor(name, MAGENTA_BLACK);
        ncPrint("   ");
        ncPrintWithColor(aux->name, CYAN_BLACK);
        ncNewline(); */
        if(strcmp(name, aux->name) == 0){
            //ncPrintWithColor("TODO OK\n", GREEN_BLACK);
            return aux;}

        aux = aux->next;
    }

    return NULL;
}

// static int semExists(char * name){
//     TSem * current = semaphoresList->first;

//     for (int i = 0; i < semaphoresList->size; i++){
//         if(strcmp(name, current->name) == FAILED)
//             return SUCCESS;
//         current = current->next;
//     }
//     return FAILED;
// }

uint64_t semOpen(char *name, int initialValue){
    _xchgLock(&semLock);

    // FIXME: Que pasa si ya existia?
    TSem * toOpen = getSem(name);
    if(toOpen == NULL){
        if(createSemaphore(name, initialValue) == FAILED){
            _unlock(&semLock);
            ncPrintWithColor("Error al crear semaforo\n", RED_BLACK);
            return FAILED;
        }
    } else {
        toOpen->openedBy++;
    }

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semClose(char * semName){
    _xchgLock(&semLock);

    TSem * toClose = getSem(semName);
    if(toClose == NULL){
        ncPrint("The Semaphore "); 
        ncPrint(semName);
        ncPrint("does not exist\n");
        return FAILED;
    }

    // ncPrint("Cerrando sem "); 
    // ncPrint(semName);
    // ncPrint("\n");

    toClose->openedBy--;
    if(toClose->openedBy == 0){
        deqSem(toClose);
        freeSem(toClose);
    }

    _unlock(&semLock);
    return SUCCESS;
}

static void freeSem(TSem * sem){
    if(sem == NULL)
        return;

    pNode * toFree = sem->firstProcess;
    for (int i = 0; i < sem->waitingProcesses; i++){
        if(toFree == NULL)
            break;
        free(toFree);
        toFree = toFree->next;        
    }
    free(sem);
}

uint64_t semWait(char * semName){
    /* ncPrintWithColor("WAIT ", CYAN_BLACK);
    ncPrint(semName);
    ncNewline(); */
    _xchgLock(&semLock);

    TSem * toWait = getSem(semName);

    if(toWait == NULL){
        ncPrint("WAIT: No existe el semaforo ");
        ncPrint(semName);
        ncNewline();
        _unlock(&semLock);
        return FAILED;
    }

    _xchgLock(&toWait->lock);
    _unlock(&semLock);

    pid_t pid = getPid();

    while(toWait->value == 0){ // tengo que bloquear el proceso  
        enqPr(toWait, pid);    // agrego el proceso en la cola del semaforo
        _unlock(&toWait->lock); // desbloqueo el lock del semaforo
        if(block(pid) == -1) {  // bloqueo el proceso
            return FAILED;}
        _xchgLock(&toWait->lock); // vuelvo a bloquear el lock semaforo
    }

    toWait->value--;     // el value era mayor a 0 -> solo lo decremento
    _unlock(&toWait->lock); 
    
    return SUCCESS;
}

uint64_t semPost(char * semName){
    /* ncPrintWithColor("POST ", CYAN_BLACK);
    ncPrint(semName);
    ncNewline(); */
    _xchgLock(&semLock);

    TSem * toPost = getSem(semName);

    if(toPost == NULL){
        ncPrint("POST: No existe el semaforo ");
        ncPrint(semName);
        ncNewline();
        _unlock(&semLock);
        return FAILED;
    }

    _xchgLock(&toPost->lock);
    _unlock(&semLock);

    toPost->value++;

    int pid = deqPr(toPost);
    if(pid == FAILED){
        _unlock(&toPost->lock);
        //ncPrint("No hay procesos para desbloquear\n");
        return SUCCESS;
    }

    _unlock(&toPost->lock);

    if(unblock(pid) == 0){
        ncPrintWithColor("MALARDO\n", RED_BLACK);
            return FAILED;}

    return SUCCESS;
}

void printListofSemaphores(){
    TSem * toPrint = semaphoresList->first;

    if(toPrint == NULL){
        ncPrint("No semaphores to show\n");
        return;
    }

    ncPrintWithColor("NAME              VALUE     BLOCKED PROCESSES\n", ORANGE_BLACK);

    for(int i = 0; i < semaphoresList->size; i++){
        ncPrint(toPrint->name);
        ncPrint("        ");
        ncPrintDec(toPrint->value);
        ncPrint("      ");

        pNode * process = toPrint->firstProcess;
        for(int j = 0; j < toPrint->waitingProcesses; j++){
            if(process == NULL)
                ncPrintWithColor("WTF", RED_BLACK);
            else
                ncPrintDec(process->pid);
            ncPrint("   ");
            process = process->next;
        }

        toPrint = toPrint->next;
        ncPrint("\n");
    }

}