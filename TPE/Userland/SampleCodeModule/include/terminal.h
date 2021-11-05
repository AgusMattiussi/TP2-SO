#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <stdio.h>
#include <strings.h>
#include <gamemode.h>
#include <drawings.h>

#define COMMANDS_COUNT 8
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

void getRegisters();

void printmem(int argSize, char *args[]);

void printTime();

void divZero();

void invalidOpCode();

void gamemode();

#endif
