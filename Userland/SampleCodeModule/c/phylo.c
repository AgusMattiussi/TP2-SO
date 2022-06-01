#include <phylo.h>

//TODO: Cambiar a static?
int phylCount = INITIAL_PHYL;
char * forks[MAX_PHYL];

void phylo() {
    for (int i = 0; i < MAX_PHYL; i++){
        char semName[3];
        itoa(i, semName);

        forks[i] = semName;
        if(sys_sem_open(forks[i], 1) != 0){
            print("MALARDO\n");
            return;
        }

    }


}