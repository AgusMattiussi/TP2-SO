#include <terminal.h>


static char * commandsNames[COMMANDS_COUNT];
static char * commandsDesc[COMMANDS_COUNT];
static void (*commandsFn[COMMANDS_COUNT])(int argSize, char *args[]);

static int commandIndex = 0;

void startTerminal(){
    startCommands();

    while(1){
        char buffer[BUFFER_SIZE] = {0};
        printWithColor("$> ", GREEN_BLACK);
        scan(buffer, BUFFER_SIZE);
        putChar('\n');
        executeCommand(buffer);
    }
    
}

void startCommands(){
    commandBuilder("help", "Displays information about every command available.", &help);
    commandBuilder("clear", "Clears the screen.", &clearScreen);
    commandBuilder("inforeg", "Displays the information of all the registers, if saved before.", &getRegisters);
    commandBuilder("printmem", "Displays a 32 bytes memory dump of the address passed as an argument", &printmem);
    commandBuilder("time", "Displays the current time and date.", &printTime);
    commandBuilder("divzero", "Displays exception of division by zero.", &divZero);
    commandBuilder("invalidopcode", "Displays exception of an invalid operation code.", &invalidOpCode);
    commandBuilder("mem", "Displays the current memory state.", &mem);
    commandBuilder("ps", "Displays a list with all running processes.", &ps);
    commandBuilder("sleep", "Delay for a specified amount of time.", &sleep);
    commandBuilder("loop", "Displays current PID with a message.", &runLoop);
    commandBuilder("nice", "Changes a process priority.", &nice);
    commandBuilder("block", "Blocks a running process given its pid.", &block);
    commandBuilder("kill", "Kills a running process given its pid.", &kill);
    commandBuilder("sem", "Displays a list with all the semaphores.", &sem);
    // commandBuilder("funca", "Con suerte crea un nuevo proceso", &help); //Cambiar a funca
    commandBuilder("testmm", "Test for the memory manager.", &test_mm);
    commandBuilder("testprocesses", "Test for the creation of processes.", &test_processes);
    commandBuilder("testpriority", "Test for the priority of the scheduler.", &test_prio);
    commandBuilder("testsynchro", "Test for the synchronization of processes.", &test_sync);
}

void commandBuilder(char *name, char *desc, void (*fn)()){
    commandsNames[commandIndex] = name;
    commandsDesc[commandIndex] = desc;
    commandsFn[commandIndex] = fn;
    commandIndex++;
}

void executeCommand(char *buffer){
    char *arguments[3];
    int argumentsCount = strtok(buffer, ' ', arguments, 3);

    if(argumentsCount <= 0 || argumentsCount > 3){
        print("Invalid amount of arguments.\n");
        return;
    }

    for(int i=0; i< COMMANDS_COUNT; i++){
        if(strcmp(buffer, commandsNames[i]) == 0){
            (*commandsFn[i])(argumentsCount - 1, arguments + 1);
            return;
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

void clearScreen(){
    clear();
}

void getRegisters(){
    sys_getRegistersInfo();
}

void printmem(int argSize, char *args[]){
    if(argSize != 1){
        print("Invalid amount of arguments.\n");
        return;
    }
    unsigned long address = hexaStringToInt(args[0]);
    sys_PrintMem((char *) address);
}

void printTime(){
    char str[3];
    int hours = sys_getTime(HOURS);

    itoa(hours, str);
    print(str);

    print(":");

    itoa(sys_getTime(MINUTES), str);
    print(str);

    print(" - ");

    itoa(sys_getTime(DAY), str);
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
    int x = 1/0;
    if(x){
    }
}

void invalidOpCode(){
    sys_raiseInvOpCodeExc();
}

void mem(){
    sys_mem();
}

void ps(){
    sys_ps();
}

void runLoop(){
    char *argv[] = {"loop"};
	sys_createProcess(&loop, 1, argv, 10);
}

void block(int argSize, char *args[]) {
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    
    unsigned long long pid = atoull(args[0]);
    
    if (sys_togglePsState(pid) == 0) {
        print("The process does not exist.\n");
    } else {
        print("Success!\n");
    }
}

void kill(int argSize, char *args[]) {
    if (argSize != 1) {
        print("Invalid amount of arguments.\n");
        return;
    }
    
    unsigned long long pid = atoull(args[0]);
    
    if (sys_killPs(pid) == 0) {
        print("The process does not exist or cannot be killed.\n");
    } else {
        print("Success!\n");
    }
}

void nice(int argSize, char *args[]){
    if (argSize != 2) {
        print("Invalid amount of arguments.\n");
        return;
    }

    unsigned long long pid = atoull(args[0]);
    char newPriority = atoull(args[1]);

    sys_nice(pid, newPriority);
}
void sem(){
    sys_sem();
}
	
