#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdio.h>
#include <strings.h>

#define COMMANDS_COUNT 7
#define BUFFER_SIZE 50

#define MINUTES 2
#define HOURS 4
#define DAY 6
#define MONTH 8
#define YEAR 9
#define LOCAL_TIME_CORRECTION 1

void startTerminal();

void startCommands();

void commandBuilder(char *name, char *desc, void (*fn)());

void executeCommand(char *buffer);

void help();

void clearScreen();

void printmem(int argSize, char *args[]);

void printTime();

void divZero();

void invalidOpCode();

#endif
