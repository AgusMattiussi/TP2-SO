#include <syscallDispatcher.h>

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	switch (rdi) {																	// ======== Origen de las llamadas ========
		case 0:
			return kb_getChar(); 													// <-- unsigned char getChar() || stdio.c
		case 1:
			ncPrintCharWithColor((char) rsi, (unsigned char)rdx); 					//< -- void putCharWithColor(char c, unsigned char colorCode) || stdio.c
			return 1;																
		case 2:
			return getTime(rsi);													// <-- int getTime(int descriptor) || stdio.c
		case 3:
			ncClear();																// <-- void clear() || stdio.c
			return 3;	
		case 4:
			saveRegisterInfo(); 														
			return 4;
		case 5:
			printMemory(rsi);														// <-- void printmem(int argSize, char *args[]) || terminal.c
			return 5;
		case 6:
			ncPrintCharInPos((char)rsi, (int)rdx, (int)rcx, (unsigned char)r8);		// <-- void printCharInPos(char c, int row, int col, unsigned char colorCode) || stdio.c
			return 6;
		case 7:
			return getDecis();														// <-- unsigned long getDeciseconds() || stdio.c
		case 8:
			getRegistersInfo(); 													// <-- void getRegisters() || terminal.c
			return 8;
		case 9:
			printAllProcessesInfo();
			return 9;
		case 10:
			return getPid();
		case 11:
			return toggleBlocked(rsi);												// <-- void block(int argSize, char *args[]) || terminal.c
		case 12:
			return kill(rsi);												        // <-- void kill(int argSize, char *args[]) || terminal.c
		case 13:
			return getSeconds();
		case 14:
			mem();
			return 14;
		case 15:
			return createProcess((void (*)(int, char **))rsi, (int)rdx, (char **)rcx, (uint8_t) r8);
		case 16:
			// printListofSemaphores();
			ncPrint("Lista de semaforos\n");
			return 16;
		case 17:
			nice(rsi, (uint8_t)rdx);
			return 17;
		case 18:
			yield();
			return 18;
		case 19:
			return (uint64_t)malloc(rsi);
		case 20:
			free((void *)rsi);
			return 20;
	}
    // Por default devuelve 0
	return 0;
}

