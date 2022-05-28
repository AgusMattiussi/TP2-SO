#include <loop.h>

void sleep(int argSize, char *args[]){
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    int secondsElapsed = sys_getSeconds();
    int final = secondsElapsed + atoull(args[0]);
    while(sys_getSeconds() <= final);
}

void loop(){
    unsigned long long pid = sys_getPid();
    char *loopTime[1] = {"5"};

    while(1){
        sleep(1, loopTime);
        print("Hola!!! Soy el proceso con PID: ");
        printInt(pid);
        putChar('\n');
    }
}