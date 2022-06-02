#include <terminal.h>


static char * commandsNames[COMMANDS_COUNT];
static char * commandsDesc[COMMANDS_COUNT];
static void (*commandsFn[COMMANDS_COUNT])(int argSize, char *args[]);
static int commandsBuiltIn[COMMANDS_COUNT];

static int commandIndex = 0;

void startTerminal(){
    startCommands();
    sys_nice(sys_getPid(), MAX_PRIORITY);
    // sys_killPs(1);
    while(1){
        char buffer[BUFFER_SIZE] = {0};
        printWithColor("$> ", GREEN_BLACK);
        scan(buffer, BUFFER_SIZE);
        putChar('\n');
        executeCommand(buffer);
    }
    
}

void startCommands(){
    commandBuilder("help", "Displays information about every command available.", &help, TRUE);
    commandBuilder("clear", "Clears the screen.", &clearScreen, TRUE);
    commandBuilder("inforeg", "Displays the information of all the registers, if saved before.", &getRegisters, TRUE);
    commandBuilder("printmem", "Displays a 32 bytes memory dump of the address passed as an argument", &printmem, TRUE);
    commandBuilder("time", "Displays the current time and date.", &printTime, TRUE);
    commandBuilder("divzero", "Displays exception of division by zero.", &divZero, TRUE);
    commandBuilder("invalidopcode", "Displays exception of an invalid operation code.", &invalidOpCode, TRUE);
    commandBuilder("mem", "Displays the current memory state.", &mem, TRUE);
    commandBuilder("ps", "Displays a list with all running processes.", &ps, TRUE);
    commandBuilder("sleep", "Delay for a specified amount of time.", &sleep, TRUE);
    commandBuilder("loop", "Displays current PID with a message.", &loop, FALSE);
    commandBuilder("nice", "Changes a process priority.", &nice, TRUE);
    commandBuilder("block", "Blocks a running process given its pid.", &block, TRUE);
    commandBuilder("kill", "Kills a running process given its pid.", &kill, TRUE);
    commandBuilder("sem", "Displays a list with all the semaphores.", &sem, TRUE);
    // commandBuilder("funca", "Con suerte crea un nuevo proceso", &help); //Cambiar a funca
    commandBuilder("testmm", "Test for the memory manager.", &test_mm, FALSE);
    commandBuilder("testprocesses", "Test for the creation of processes.", &test_processes, FALSE);
    commandBuilder("testpriority", "Test for the priority of the scheduler.", &test_prio, FALSE);
    commandBuilder("testsynchro", "Test for the synchronization of processes.", &test_sync, FALSE);
    commandBuilder("cat", "Displays the input on the screen.", &cat, FALSE);
    commandBuilder("wc", "Displays the quantity of lines of the input.", &wc, FALSE);
    commandBuilder("filter", "Filters the vowels of the input.", &filter, FALSE);
    commandBuilder("pipe", "Displays a list with all the pipes.", &pipe, TRUE);
}

void commandBuilder(char *name, char *desc, void (*fn)(), int builtin){
    commandsNames[commandIndex] = name;
    commandsDesc[commandIndex] = desc;
    commandsFn[commandIndex] = fn;
    commandsBuiltIn[commandIndex] = builtin;
    commandIndex++;
}

void executeCommand(char *buffer){
    char *arguments[3];
    int argumentsCount = strtok(buffer, ' ', arguments, 3);

    if(argumentsCount <= 0 || argumentsCount > 5){
        print("Invalid amount of arguments.\n");
        return;
    }

    for(int i=0; i< COMMANDS_COUNT; i++){
        if(strcmp(arguments[0], commandsNames[i]) == 0){
            
            if(commandsBuiltIn[i]){
                (*commandsFn[i])(argumentsCount - 1, arguments + 1);
                return;

            } else {
                // print("Not built in command.\n");
                // context * context = sys_malloc(sizeof(context));
                mode processMode = FOREGROUND;

                if(argumentsCount == 2 && arguments[1][0] == '-')
                    processMode = BACKGROUND;
                    // print("Background run\n");
                
                if(argumentsCount == 3 && arguments[1][0] == '/'){ //chequear 2do argumento post pipe
                    // print("IPC\n");
                    for(int j=0; j< COMMANDS_COUNT; j++){
                        if(strcmp(arguments[2], commandsNames[j]) == 0){
                            //TODO: pipes 
                            // int fd = sys_pipeOpen("|");
                            // if(fd == -1){
                            //     print("Pipe opening error\n");
                            //     return;
                            // }
                            // context->stdIn = fd;
                            // context->stdOut = 0;
                            // int p1 = sys_createProcess(commandsFn[i], 1, commandsNames[i], 10, context);
                            // context->stdIn = 0;
                            // context->stdOut = fd;
                            // int p2 = sys_createProcess(commandsFn[j], 1, commandsNames[j], 10, context);
                            // sys_kill(p1);
                            // sys_pipeClose(fd);
                            print("Todo Ok\n");
                        }
                    }         
                } else
                    sys_createProcess(commandsFn[i], 1, arguments, NULL, processMode);
                return;
            }
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
	sys_createProcess(&loop, 1, argv, NULL, FOREGROUND);
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
        print("The process does not exist.\n");
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

void runCat(){
    char *argv[] = {"cat"};
	sys_createProcess(&cat, 1, argv, NULL, FOREGROUND);
}

void runWc(){
    char *argv[] = {"wc"};
	sys_createProcess(&wc, 1, argv, NULL, FOREGROUND);
}

void runFilter(){
    char *argv[] = {"filter"};
	sys_createProcess(&filter, 1, argv, NULL, FOREGROUND);
}

void pipe(){
    sys_pipe();
}
