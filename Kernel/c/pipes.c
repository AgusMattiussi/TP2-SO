#include <pipes.h>

static void enqPipe(TPipe * pipe);
static TPipe * deqPipe();
static TPipe * getPipe(char * name);
static int * createPipe(char *name, int id);
static int generateNewFd();

static char * handlerSemName = "pipesHandler";
static pipeList * pipesList;
static int lastFdGenerated = 2;

void initPipes(){
    if(semOpen(handlerSemName, 1) == FAILED){
        ncPrint("ERROR: failed to open semaphore in 'initPipes()'\n");
        return;
    }
    pipesList = malloc(sizeof(pipeList));
    pipesList->first = NULL;
    pipesList->last = NULL;
    pipesList->size = 0;
}

static void enqPipe(TPipe * pipe) {
    if(pipe == NULL || pipesList == NULL)
        return;
    
    if(pipesList->first == NULL)
        pipesList->first = pipe;
    else
        pipesList->last->next = pipe;
    
    pipesList->last = pipe;

    pipe->next = NULL;

    pipesList->size++;
}

static TPipe * deqPipe() {

    if(pipesList == NULL || pipesList->size == 0)
        return NULL;

    TPipe * deq = pipesList->first;

    if(pipesList->size == 1){
        pipesList->first = NULL;
        pipesList->last = NULL;
    } else {
        pipesList->first = deq->next;
    }

    deq->next = NULL;

    pipesList->size--;
    return deq;
}

static int * createPipe(char *name, int id){
    TPipe* new = malloc(sizeof(TPipe));

    if(new == NULL)
        return NULL;

    new->fds[0] = generateNewFd();
    new->fds[1] = generateNewFd();

    memcpy(new->name, name, strlen(name)+1);

    new->readIndex = 0;
    new->writeIndex = 0;

    char rSem[NAME_MAX_SIZE];
    int lenght = strlen(READ_SEM_NAME);
    memcpy(rSem, READ_SEM_NAME, lenght+1);
    rSem[lenght] = id + '0';
    rSem[lenght+1] = 0;
    int sr = semOpen(rSem, 0);

    char wSem[NAME_MAX_SIZE];
    lenght = strlen(WRITE_SEM_NAME);
    memcpy(wSem, WRITE_SEM_NAME, lenght+0);
    wSem[lenght] = id + '0';
    wSem[lenght+1] = 0;
    int sw = semOpen(wSem, 1);

    if(sr == FAILED || sw == FAILED){
        ncPrint("Error abriendo semaforos en createPipe\n");
        return NULL;
    }

    memcpy(new->readSemName, rSem, strlen(rSem)+1);
    memcpy(new->writeSemName, wSem, strlen(wSem)+1);

    new->numOfProcessesAttached = 0;


    enqPipe(new);
    return new->fds;
}

static TPipe * getPipe(char * name){
    TPipe * aux = pipesList->first;

    for(int i = 0; i < pipesList->size; i++){
        if(strcmp(name, aux->name) == 0){
            return aux;}
        aux = aux->next;
    }
    return NULL;
}

int * pipeOpen(char *name){
    if (semWait(handlerSemName) == FAILED){
        ncPrint("Error semWait en pipeOpen\n");
        return FAILED;
    }

    int * fds;
    TPipe * toOpen = getPipe(name);
    if (toOpen == NULL){
        fds = createPipe(name, pipesList->size+1);
        if(fds == NULL){
            ncPrint("Error creando pipe en pipeOpen\n");
        }
    } else{
        toOpen->numOfProcessesAttached++;
        fds = toOpen->fds;
    }
    
    semPost(handlerSemName);
    return fds;
}

uint64_t pipeClose(char * pipeName){
    if (semWait(handlerSemName) == FAILED){
        ncPrint("Error semWait en pipeClose\n");
        return FAILED;
    }

    TPipe * toClose = getPipe(pipeName);
    if(toClose == NULL){
        ncPrint("The pipe "); 
        ncPrint(pipeName);
        ncPrint("does not exist\n");
        return FAILED;
    }

    uint64_t rs = semClose(toClose->readSemName);
    uint64_t rw = semClose(toClose->writeSemName);

    if(rs == FAILED || rw == FAILED){
        ncPrint("Error semClose en pipeClose\n");
        return FAILED;
    }

    deqPipe(toClose);
    free(toClose);
    // semPost(handlerSemName);
    if (semPost(handlerSemName) == FAILED){
        ncPrint("Error semPost en pipeClose\n");
        return FAILED;
    }
    return SUCCESS;
}

uint64_t writePipe(char * pipeName, char *str){
    TPipe * toWrite = getPipe(pipeName);
    if(toWrite == NULL){
        ncPrint("The pipe "); 
        ncPrint(pipeName);
        ncPrint("does not exist\n");
        return FAILED;
    }

    while(*str != 0){
        if(writeCharInPipe(toWrite, *str++) == FAILED)
            return FAILED;
    }

    return SUCCESS;
}

uint64_t writeCharInPipe(TPipe * pipe, char c){
    if(semWait(pipe->writeSemName) == FAILED){
        ncPrint("Error semWait en writeCharInPipe\n");
        return FAILED;
    }

    pipe->buffer[pipe->writeIndex % BUFFER_SIZE] = c;
    pipe->writeIndex++;

    if(semPost(pipe->writeSemName) == FAILED){
        ncPrint("Error semPost en writeCharInPipe\n");
        return FAILED;
    }

    return SUCCESS;
}

char readPipe(char * pipeName){
    TPipe * toRead = getPipe(pipeName);
    if(toRead == NULL){
        ncPrint("The pipe "); 
        ncPrint(pipeName);
        ncPrint("does not exist\n");
        return FAILED;
    }

    if(semWait(toRead->readSemName) == FAILED){
        ncPrint("Error semWait en readPipe\n");
        return FAILED;
    }

    char toRet = toRead->buffer[toRead->readIndex % BUFFER_SIZE];
    toRead->readIndex++;

    if(semPost(toRead->readSemName) == FAILED){
        ncPrint("Error semPost en readPipe\n");
        return FAILED;
    }

    return toRet;
}

void printListOfPipes(){
    TPipe * toPrint = pipesList->first;

    if(toPrint == NULL){
        ncPrint("No pipes to show\n");
        return;
    }
    ncPrintWithColor("NAME\n", ORANGE_BLACK);
    for(int i = 0; i < pipesList->size; i++){
        ncPrint(toPrint->name);

        toPrint = toPrint->next;
        ncPrint("\n");
    }
}

static int generateNewFd(){
    return lastFdGenerated++;
}