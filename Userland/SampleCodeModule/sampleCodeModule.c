/* sampleCodeModule.c */
#include <terminal.h>


void paredesufrir () {
    /* for (int i = 0; i < 5; i++)
        print("HOLAXD\n "); */
	while(1);
}
    
int main() {
	char *argv[] = {"Terminal"};
	//char *argv2[] = {"paredesufrir"};
	sys_createProcess(&startTerminal, 1, argv, 0);
	//sys_createProcess(&paredesufrir, 1, argv2, 19);
	//sys_createProcess(&paredesufrir, 1, argv2, 19);
	//sys_createProcess(&paredesufrir, 1, argv2);
	//sys_createProcess(&paredesufrir, 1, argv2);
	// startTerminal();
	// while (1);
	
	return 0;
}
