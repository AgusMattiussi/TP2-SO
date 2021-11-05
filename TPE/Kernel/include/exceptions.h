#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

#define IP_INDEX 15
#define SP_INDEX 18

#include <naiveConsole.h>
#include <stdint.h>

typedef struct initialState{
    uint64_t IP;
    uint64_t SP;
}initialState;

extern uint64_t getSP();

//extern void movrax();

extern void getRegistersInfo();

void exceptionDispatcher(int exception, uint64_t *registers);

void zeroDivision();

void invalidOpcode();

void saveInitialState(uint64_t IP, uint64_t SP);

void restartTerminal(uint64_t *registers);

#endif