#include <terminal.h>
#include <stdio.h>
#include <strings.h>

#define COMMANDS_COUNT 6
#define BUFFER_SIZE 50

#define MINUTES 2
#define HOURS 4
#define DAY 6
#define MONTH 8
#define YEAR 9
#define LOCAL_TIME_CORRECTION 1

static char * commandsNames[COMMANDS_COUNT];
static char * commandsDesc[COMMANDS_COUNT];
static void (*commandsFn[COMMANDS_COUNT])();


void startTerminal(){
    startCommands();
    char buffer[BUFFER_SIZE];
    while(strcmp(buffer, "exit") != 0){
        printWithColor("$> ", GREEN_BLACK);
        scan(buffer, BUFFER_SIZE);
        putChar('\n');
        executeCommand(buffer);
    }
}

void startCommands(){
    commandBuilder("help", "Displays information about every command available.", &help, 0);
    commandBuilder("inforeg", "Displays the information of all the registers.", &inforeg, 1);
    commandBuilder("printmem", "Displays a 32 bytes memory dump of the address passed as an argument", &printmem, 2);
    commandBuilder("time", "Displays the current time and date.", &printTime, 3);
    commandBuilder("divZero", "Displays exception of division by zero.", &divZero, 4);
    commandBuilder("invalidOpCode", "Displays exception of an invalid operation code.", &invalidOpCode, 5);
}

void commandBuilder(char *name, char *desc, void (*fn)(), int index){
    commandsNames[index] = name;
    commandsDesc[index] = desc;
    commandsFn[index] = fn;
}

void executeCommand(char *buffer){
    for(int i=0; i< COMMANDS_COUNT; i++){
        if(strcmp(buffer, commandsNames[i]) == 0){
            return (*commandsFn[i])();
        }
    }
    print("Invalid command.\n");
}

void help(){
    for(int i=0; i < COMMANDS_COUNT; i++){
        printWithColor(commandsNames[i], YELLOW_BLACK);
        print(" - ");
        print(commandsDesc[i]);
        putChar('\n');
    }
}

void inforeg(){

}

void printmem(int argSize, char *args[]){

}

void printTime(){
    char str[3];
    int hours = sys_getTime(HOURS);

    if(hours>3)
        hours -=3;
    if(hours==0)
        hours=21;
    if(hours==1)
        hours=22;
    if(hours==2)
        hours=23;

    itoa(hours, str);
    print(str);

    print(":");

    itoa(sys_getTime(MINUTES), str);
    print(str);

    print(" - ");

    itoa(sys_getTime(DAY)-LOCAL_TIME_CORRECTION, str);
    print(str);

    print("/");

    itoa(sys_getTime(MONTH), str);
    print(str);

    print("/");

    itoa(sys_getTime(YEAR), str);
    print(str);

    putChar('\n');
}

void divZero(){

}

void invalidOpCode(){

}

