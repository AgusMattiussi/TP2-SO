#include <phylo.h>

void phylo_main();
void philosopher(int argc, char ** argv);
static void eat(int id);
static void leave(int id);
static void printStatus();

static int philoCount = INITIAL_PHYL;
static char forks[MAX_PHYL][3];
static char status[MAX_PHYL];
static unsigned long long philoPids[MAX_PHYL] = {0};

void phylo_main() {
    for (int i = 0; i < MAX_PHYL; i++){
        char semName[3] = {0, 0, 0};
        itoa(i, semName);

        forks[i][0] = semName[0];
        forks[i][1] = semName[1];
        forks[i][2] = semName[2];
        //printWithColor(forks[i], RED_BLACK);
        
        if(sys_sem_open(forks[i], 1) == 0){
            print("Error creando tenedores\n");

            for (int j = 0; j < i; j++)
                sys_sem_close(forks[j]);  
            return;
        }
        //print(" ");
        //printWithColor(forks[i], MAGENTA_BLACK);
        //print("\n");
    }

    //TODO: Si falla, cerrar todo
    sys_sem_open(ROOM_SEM_NAME, philoCount-1);

    /* for (int i = 0; i < MAX_PHYL; i++){
        printWithColor(forks[i], GREEN_BLACK);
        print("\n");
    } */
    

    for (int i = 0; i < MAX_PHYL; i++){
        status[i] = '.';
    }
    printStatus();

    for (int i = 0; i < philoCount; i++){
        char id[] = {i+'0', 0};
        char * philoArgv[] = {"philosopher", id};

        //TODO: Si falla, cerrar todo
        philoPids[i] = sys_createProcess(&philosopher, 2, philoArgv, NULL, FOREGROUND);
        if(philoPids[i] == 0){
            print("Error creando filosofos\n");
        }
    }
    
    for (int i = 0; i < philoCount; i++){
        sys_wait(philoPids[i]);
        /* printWithColor("Termino ", GREEN_BLACK);
        printInt(philoPids[i]); */
        print("\n");
    }
    //while(1);
    
    for (int i = 0; i < MAX_PHYL; i++)
        sys_sem_close(forks[i]);
    sys_sem_close(ROOM_SEM_NAME);
}

void philosopher(int argc, char ** argv){
    
    int id = atoi(argv[1]);
    //printInt(id);
    int lFork = id;
    int rFork = (id+1) % philoCount;

    sys_sem_wait(ROOM_SEM_NAME);
    /* print("ENTRE ");
    print(argv[1]);
    print("\n"); */
    sys_sem_wait(forks[lFork]);
    /* print("(");
    print(argv[1]);
    print(") : ");
    print("TOME EL SEM ");
    print(forks[lFork]);
    print("\n"); */
    sys_sem_wait(forks[rFork]);
    /* print("(");
    print(argv[1]);
    print(") : ");
    print("TOME EL SEM ");
    print(forks[rFork]);
    print("\n"); */
    eat(id);
    leave(id);

    sys_sem_post(forks[lFork]);
    /* print("(");
    print(argv[1]);
    print(") : ");
    print("Libere EL SEM ");
    print(forks[lFork]);
    print("\n"); */
    sys_sem_post(forks[rFork]);
    /* print("(");
    print(argv[1]);
    print(") : ");
    print("Libere EL SEM ");
    print(forks[rFork]);
    print("\n"); */
    sys_sem_post(ROOM_SEM_NAME);
    /* print("SALI ");
    print(argv[1]);
    print("\n"); */
}

static void eat(int id){
    status[id] = 'E';
    printStatus();
    sys_yield();
}

static void leave(int id){
    status[id] = 'X';
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