#include <semaphores.h>

static int createSemaphore(char *name, int initialValue);
static void enqSem(TSem * sem);
static TSem * deqSem();
static TSem * getSem(char * name);

static uint64_t semLock;
static semList * semaphoresList;

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
    sem->next = semaphoresList->first;

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
        semaphoresList->last->next = semaphoresList->first;
    }

    deq->next = NULL;

    semaphoresList->size--;
    return deq;
}

static TSem * getSem(char * name){
    TSem * aux = semaphoresList->first;

    for(int i = 0; i < semaphoresList->size; i++){

        if(strcmp(name, aux->name) == 0)
            return aux;

        aux = aux->next;
    }

    return NULL;
}

uint64_t semOpen(char *name, int initialValue){
    _xchgLock(&semLock);

    if(createSemaphore(name, initialValue) == FAILED){
        _unlock(&semLock);
        ncPrintWithColor("Error al crear semaforo\n", RED_BLACK);
        return -1;
    }

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semClose(TSem * sem){
    _xchgLock(&semLock);

    TSem * toClose = getSem(sem);

    deqSem(toClose);

    free(toClose);

    _unlock(&semLock);
    return SUCCESS;
}

uint64_t semWait(TSem * sem){

    TSem * toWait = getSem(sem);
    pid_t pid = getPid();

    while(toWait->value == 0){ // tengo que bloquear el proceso
        
        //TODO: Agregar proceso a lista del semaforo

        if(block(pid) == -1)
            return FAILED;
    }

    toWait->value--;
    _unlock(&toWait->lock);

    return SUCCESS;
}

uint64_t semPost(TSem * sem){
    TSem * toPost = getSem(sem);

    toPost->value++;

    // TODO: Desbloquear proceso de la lista del semaforo

    _unlock(&toPost->lock);

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
        ncPrint("    ");
        ncPrintDec(toPrint->value);
        ncPrint("    ");

        pList * process = toPrint->firstProcess;
        for(int j = 0; j < toPrint->waitingProcesses; j++){
            ncPrintDec(process->pid);
            ncPrint("   ");
            process = process->next;
        }

        ncPrint("---------------------------------------------");
    }

}