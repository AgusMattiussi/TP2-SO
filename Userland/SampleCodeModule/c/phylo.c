#include <phylo.h>

void phylo_main();
void philosopher(int argc, char ** argv);
static void eat(int id);
static void leave(int id);
static void printStatus();

//TODO: Cambiar a static?
static int philoCount = INITIAL_PHYL;
static char * forks[MAX_PHYL];
static char status[MAX_PHYL];
static unsigned long long philoPids[MAX_PHYL] = {0};

void phylo_main() {
    for (int i = 0; i < MAX_PHYL; i++){
        char semName[3];
        itoa(i, semName);

        forks[i] = semName;
        if(sys_sem_open(forks[i], 1) != 0){
            print("Error creando tenedores\n");

            for (int j = 0; j < i; i++)
                sys_sem_close(forks[j]);  
            return;
        }
    }

    //TODO: Si falla, cerrar todo
    sys_sem_open(ROOM_SEM_NAME, philoCount-1);

    for (int i = 0; i < MAX_PHYL; i++){
        status[i] = '.';
    }
    printStatus();

    for (int i = 0; i < philoCount; i++){
        char id[] = {i+'0', 0};
        char * philoArgv[] = {"philosopher", id};

        //TODO: Si falla, cerrar todo
        philoPids[i] = sys_createProcess(&philosopher, 2, philoArgv, 0);
        if(philoPids[i] == 0){
            print("Error creando filosofos\n");
        }
    }
    
    /* for (int i = 0; i < MAX_PHYL; i++){
        sys_wait(philoPids[i]);
    } */
    while(1);
    
    for (int i = 0; i < MAX_PHYL; i++)
        sys_sem_close(forks[i]);
    sys_sem_close(ROOM_SEM_NAME);
}

void philosopher(int argc, char ** argv){
    
    int id = atoi(argv[1]);
    printInt(id);
    int lFork = id;
    int rFork = (id+1) % philoCount;

    //sys_sem_wait(ROOM_SEM_NAME);

    sys_sem_wait(forks[lFork]);
    sys_sem_wait(forks[rFork]);

    eat(id);
    leave(id);

    sys_sem_post(forks[lFork]);
    sys_sem_post(forks[rFork]);

    //sys_sem_post(ROOM_SEM_NAME);
}

static void eat(int id){
    status[id] = 'E';
    printStatus();
    sys_yield();
}

static void leave(int id){
    status[id] = '.';
    printStatus();
}

static void printStatus(){
    for (int i = 0; i < philoCount; i++){
        putCharWithColor(status[i], MAGENTA_BLACK);
        putChar(' ');
    }
    putChar('\n');
}

/* TODO: aniadir un filosofo es hacer post del sem de room */