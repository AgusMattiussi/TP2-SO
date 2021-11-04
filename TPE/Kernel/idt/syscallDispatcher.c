#include <syscallDispatcher.h>

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	switch (rdi) {
		case 0:
			return kb_getChar(); // getChar
		case 1:
			ncPrintCharWithColor((char) rsi, (unsigned char)rdx); //putCharWC
			return 1;
		case 2:
			return getTime(rsi);
		case 3:
			ncClear();
			return 3;	
		case 4:
			getRegistersInfo();
			return 4;
	}

    // Por default devuelve 0
	return 0;
}

