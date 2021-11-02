#include <syscallDispatcher.h>

uint64_t syscallDispatcher(uint64_t rax) {
	switch (rax) {
		case 0:
			return kb_getChar(); // getChar del Keyboard
			break;

	}

    // Por default devuelve 0
	return 0;
}

