/* sampleCodeModule.c */
#include <terminal.h>

int main() {
	char *argv[] = {"Terminal"};
	sys_createProcess(&startTerminal, 1, argv);
	// startTerminal();
	return 0;
}
