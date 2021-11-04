#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdio.h>

void startTerminal();

void startCommands();

void commandBuilder(char *name, char *desc, void (*fn)(), int index);

void executeCommand(char *buffer);

void help();

void inforeg();

void printmem(int argSize, char *args[]);

void printTime();

void divZero();

void invalidOpCode();

#endif
