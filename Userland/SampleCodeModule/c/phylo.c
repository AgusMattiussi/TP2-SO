#include <phylo.h>

//TODO: Cambiar a static?
int phylCount = INITIAL_PHYL;
char * forks[MAX_PHYL];

void phylo() {
    for (int i = 0; i < MAX_PHYL; i++){
        char semName[3];
        itoa(i, semName);
        forks[i] = semName;

        printWithColor(forks[i], RED_BLACK);
        print("\n");
    }

}