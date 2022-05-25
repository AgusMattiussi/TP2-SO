#ifndef _STDIO_H
#define _STDIO_H

#define BACKSPACE '\b'
#define SHIFT 201

// COLORES: El formato es COLORLETRA_COLORFONDO
#define BLACK_BLACK 0x00
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F
#define RED_RED 0x44
#define MAGENTA_MAGENTA 0x55
#define WHITE_WHITE 0xFF
#define LIGHTGREEN_LIGHTGREEN 0xAA
#define BLUE_LIGHTGREEN 0xA1
#define BLACK_LIGHTGREEN 0xA0
#define WHITE_RED 0x4F
#define YELLOW_RED 0x4E
#define BLUE_BLUE 0x11
#define BLUE_GREY 0x71
#define BLUE_WHITE 0xF1
#define GREY_GREY 0x77
#define RED_WHITE 0xF4
#define RED_GREY 0x74

#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

/**
 * Syscall: Devuelve un caracter ingresado por el usuario o 0 en
 * su defecto
**/
extern char sys_getChar();

/**
 * Syscall: Ubica el caracter 'c' en la posicion actual de la pantalla
 * con el color indicado en 'colorCode'
**/
extern void sys_putCharWC(char c, unsigned char colorCode);

/**
 * Syscall: Devuelve el campo de la fecha/hora actual indicado en 'descriptor'
**/
extern int sys_getTime(int descriptor);

/**
 * Syscall: Imprime en pantalla cada registro del procesador con su contenido
**/
extern void sys_getRegistersInfo();

/**
 * Syscall: Elimina todo el contenido de la pantalla y posiciona el prompt al comienzo
 * de la misma
**/
extern void sys_clear();

/**
 * Syscall: Imprime en pantalla el contenido de 32 bytes de memoria a partir de la
 * direccion 'address'
**/
extern void sys_PrintMem(char * address);

/**
 * Syscall: Imprime el caracter 'c' en la fila 'row' y columna 'col' de la pantalla,
 * teniendo en cuenta que la misma tiene un tamaño de 25x80. Ademas, lo hace en el
 * color indicado por 'colorCode'
**/
extern void sys_printCharInPos(char c, int row, int col, unsigned char colorCode);

/**
 * Syscall: Retorna los decisegundos desde que se inicio el sistema
**/
extern unsigned long sys_getDeciseconds();

/**
 * Syscall: Dispara la excepcion de Operando Invalido
**/
extern void sys_raiseInvOpCodeExc();

/**
 * Syscall: Crea un backup de los registros para luego imprimirlos cuando se llame
 * a 'inforeg'
**/
extern void sys_saveRegs();

/**
 * Syscall: Imprime el estado actual de la memoria
**/
extern void sys_mem();

/**
 * Syscall: Imprime una lista con todos los procesos en ejecución
**/
extern void sys_ps();

/**
 * Syscall: Retorna el Process ID del proceso actual
**/
extern unsigned long long sys_getPid();

/**
 * Syscall: Cambia el estado de un proceso entre bloqueado y listo dado su ID
**/
extern int sys_togglePsState(unsigned long long pid);

/**
 * Syscall: Mata un proceso dado su ID
**/
extern int sys_killPs(unsigned long long pid);

/**
 * Syscall: Retorna los segundos desde que se inicio el sistema
**/
extern int sys_getSeconds();

/**
 * Devuelve un caracter ingresado por el usuario siempre que este no
 * sea nulo (distinto de 0)
**/
unsigned char getChar();

/**
 * Devuelve un caracter ingresado por el usuario o en su defecto 0. Se
 * utiliza para que la ejecucion de funciones como la impresion de la hora
 * y el cronometro puedan seguir su ejecucion aunque el usuario no haya
 * ingresado ningun caracter (es decir, cuando sys_getChar devuelve 0)
**/
unsigned char getCharOrNull();

/**
 * Imprime en la posicion actual de la pantalla el string pasado por
 * parametro
**/
void print(char * string);

/**
 * Imprime en pantalla el caracter 'c' con el color predeterminado.
**/
void putChar(char c);

/**
 * Wrapper de sys_putCharWC
**/
void putCharWithColor(char c, unsigned char colorCode);

/**
 * Recibe hasta 'size' caracteres ingresados por el usuario y los guarda en
 * 'buffer' mientras que estos sean distintos que '\n'. Cuando se recibe dicho
 * char, se cierra el string 'buffer' y se retorna la cantidad de caracteres
 * leidos
**/
int scan(char *buffer, int size);

/**
 * Imprime en la posicion actual de la pantalla el string pasado por
 * parametro y en el color indicado por 'colorCode'
**/
void printWithColor(char * string, unsigned char colorCode);

/**
 * Wrapper de sys_clear
**/
void clear();

/**
 * Wrapper de sys_getTime
**/
int getTime(int descriptor);

/**
 * Wrapper de sys_getDeciseconds
**/
unsigned long getDeciseconds();

/**
 * Imprime el string pasado por parametro a partir de la fila 'row' y la columna
 * 'col' de la misma en el color indicado por 'colorCode'
**/
void printInPos(char * string, int row, int col, unsigned char colorCode);

/**
 * Wrapper de sys_printCharInPos
**/
void printCharInPos(char c, int row, int col, unsigned char colorCode);

#endif
