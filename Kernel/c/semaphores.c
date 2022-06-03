#include <semaphores.h>

static int createSemaphore(char *name, int initialValue);
static void enqSem(TSem * sem);
static TSem * deqSem();
static void enqPr(TSem * sem, pid_t pid);
static pid_t deqPr(TSem * sem);
static TSem * getSem(char * name);

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

    memcpy(new->name, name, strlen(name));

    new->value = initialValue;
    new->lock = 0;
    new->firstProcess = NULL;
    new->lastProcess = NULL;
    new->waitingProcesses = 0;

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

uint64_t semOpen(char *name, int initialValue){
    _xchgLock(&semLock);

    if(createSemaphore(name, initialValue) == FAILED){
        _unlock(&semLock);
        ncPrintWithColor("Error al crear semaforo\n", RED_BLACK);
        return FAILED;
    }

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semClose(char * semName){
    _xchgLock(&semLock);

    TSem * toClose = getSem(semName);

    deqSem(toClose);

    free(toClose);

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semWait(char * semName){
    /* ncPrintWithColor("WAIT ", CYAN_BLACK);
    ncPrint(semName);
    ncNewline(); */
    _xchgLock(&semLock);

    TSem * toWait = getSem(semName);

    if(toWait == NULL){
        _unlock(&semLock);
        ncPrint("WAIT: No existe el semaforo ");
        ncPrint(semName);
        ncNewline();
        return FAILED;
    }

    _xchgLock(&toWait->lock);
    _unlock(&semLock);

    pid_t pid = getPid();

    while(toWait->value == 0){ // tengo que bloquear el proceso  
        enqPr(toWait, pid);    // agrego el proceso en la cola del semaforo
        _unlock(&toWait->lock); // desbloqueo el lock del semaforo
        if(block(pid) == -1)   // bloqueo el proceso
            return FAILED;
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
        _unlock(&semLock);
        ncPrint("POST: No existe el semaforo ");
        ncPrint(semName);
        ncNewline();
        return FAILED;
    }

    _xchgLock(&toPost->lock);
    _unlock(&semLock);

    toPost->value++;

    int pid = deqPr(toPost);
    if(pid == FAILED){
        _unlock(&toPost->lock);
        _unlock(&semLock);
        //ncPrint("No hay procesos para desbloquear\n");
        return FAILED;
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

    ncPrintWithColor("NAME    VALUE     BLOCKED PROCESSES\n", ORANGE_BLACK);

    for(int i = 0; i < semaphoresList->size; i++){
        ncPrint(toPrint->name);
        ncPrint("        ");
        ncPrintDec(toPrint->value);
        ncPrint("      ");

        pNode * process = toPrint->firstProcess;
        for(int j = 0; j < toPrint->waitingProcesses; j++){
            ncPrintDec(process->pid);
            ncPrint("   ");
            process = process->next;
        }

        toPrint = toPrint->next;
        ncPrint("\n");
    }

}