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
#define LOCAL_DATE_CORRECTION 6

/**
 * Inicializa la terminal, cargando los comandos y entrando en un ciclo infinito
**/
void startTerminal();

/**
 * Carga los diferentes comandos en tres vectores paralelos mediante un llamados a commandBuilder 
 * que contienen el nombre, la descripcion y la funcion de cada uno
**/
void startCommands();

/**
 * Carga el nombre, la descrición y la función de un comando en su respectivo vector
**/
void commandBuilder(char *name, char *desc, void (*fn)());

/**
 * Ejecuta el comando ingresado, primero lo tokeniza separando por espacios para validar
 * que tenga la cantidad de argumentos correpondiente, si no es asi imprime un mensaje de error al
 * igual que si se ingreso un comando invalido
**/
void executeCommand(char *buffer);

/**
 * Funcion del comando 'help'. Despliega en pantalla todos los comandos disponibles para el usuario 
 * con su respectiva descripción.
**/
void help();

/**
 * Función del comando 'clear'. Limpia la pantalla dejando a la terminal vacía
**/
void clearScreen();

/**
 * Función del comando 'inforeg'. Despliega por pantalla el valor de los registros al momento del llamado
**/
void getRegisters();

/**
 * Función del comando 'printmem'. Realiza un dump de 32 bytes en memoria de una dirección de memoria pasada por parametro
**/
void printmem(int argSize, char *args[]);

/**
 * Función del comando 'time'. Imprime en pantalla la hora y la fecha actual
**/
void printTime();

/**
 * Función del comando 'divzero'. Verifica el correcto funcionamiento de la excepción de división por cero.
 **/
void divZero();

/**
 * Función del comando 'invalidopcode'. Verifica el correcto funcionamiento de código de operación invalido.
**/
void invalidOpCode();

/**
 * Función del comando 'gamemode'. Divide la pantalla en cuatro: 
 * en una zona muestra la hora en formato hh:mm:ss, en otra hay un cronómetro funcional y en las restantes 
 * se puede jugar al sudoku y al ahorcado respectivamente.
**/
void gamemode();

#endif
