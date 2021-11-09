#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define TOTAL_SCREEN_LENGTH width*height*2
#define LINE_LENGTH width*2
#define COL_LENGTH height*2
#define LAST_LINE video + TOTAL_SCREEN_LENGTH - LINE_LENGTH
#define DEFAULT_COLOR WHITE_BLACK
#define PROMPT_SIZE 5 // chars que ocupa el prompt

// Colores. El formato es COLORLETRA_COLORFONDO
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F

/**
 * Imprime el string pasado como parametro a partir de la posicion actual de la pantalla
**/
void ncPrint(const char * string);

/**
 * Imprime el caracter pasado como parametro en la posicion actual de la pantalla
**/
void ncPrintChar(char character);

/**
 * Imprime el string pasado como parametro a partir de la posicion indicada por 
 * row (fila) y col (columna) y en el color indicado en colorCode
**/
void ncPrintInPos(const char * string, int row, int col, uint8_t colorCode);

/**
 * Imprime el caracter pasado como parametro en la posicion indicada por 
 * row (fila) y col (columna) y en el color indicado en colorCode
**/
void ncPrintCharInPos(char c, int row, int col, uint8_t colorCode);

/**
 * Imprime el string pasado como parametro a partir de la posicion actual de la pantalla
 * y con el color indicado en colorCode
**/
void ncPrintWithColor(const char * string, uint8_t colorCode);

/**
 * Imprime el caracter pasado como parametro en la posicion actual de la pantalla
 * y en el color indicado por colorCode
**/
void ncPrintCharWithColor(char character, uint8_t colorCode);

/**
 * Genera un salto de linea (llena lo que quede de la linea con ' ')
**/
void ncNewline();

/**
 * Limpia la pantalla (comenta toda la pantalla con ' ')
**/
void ncClear();

/**
 * Mueve una linea hacia arriba todos los caracteres de la
 * pantalla
**/
void ncScrollUp();

/**
 * Elimina el caracter de la posicion anterior en la pantalla (lo reemplaza por ' ')
**/
void ncBackspace();

/**
 * Funciones para imprimir un entero pasado por parametro en formato decimal,
 * hexadecimal o binario respectivamente
**/
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);

/**
 * Imprime el entero pasado por parametro en la base tambien pasada por parametro
**/
void ncPrintBase(uint64_t value, uint32_t base);

/**
 * Convierte el entero 'value' a la base 'base' y guarda el resultado como string en 'buffer'.
 * Ademas devuelve la cantidad de digitos del nuevo numero.
**/
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);



#endif