#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>
#include <naiveConsole.h>

#define REGISTERS_COUNT 16

/**
 * Imprime cada registro con su contenido
**/
void inforeg(uint64_t * regInfo);

/**
 * Imprime 32 bytes a partir de la direccion de memoria pasada
 * por parametro
**/
void printMemory(uint64_t argument);

#endif