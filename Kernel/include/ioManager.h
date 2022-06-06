#ifndef _IO_MANAGER_H
#define _IO_MANAGER_H

#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <pipes.h>

#define STDIN 0
#define STDOUT 1

uint32_t readHandler(int fd);
uint32_t writeHandler(int fd, char * str);

#endif