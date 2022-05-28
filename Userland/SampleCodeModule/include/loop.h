#ifndef LOOP_H
#define LOOP_H

#include <stdio.h>
#include <strings.h>

/**
 * Función del comando 'sleep'. Retrasa el funcionamiento de la terminal por un tiempo especifico.
**/
void sleep(int argSize, char *args[]);

/**
 * Función del comando 'loop'. Imprime el ID del proceso actual con un saludo cada una cantidad de segundos.
**/
void loop();

#endif