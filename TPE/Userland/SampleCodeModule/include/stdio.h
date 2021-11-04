#ifndef _STDIO_H
#define _STDIO_H

extern char sys_getChar();
extern void sys_putCharWC(char c, unsigned char colorCode);

char getChar();
void print(char * string);
void putChar(char c);
void putCharWithColor(char c, unsigned char colorCode);
int scan(char *buffer, int size);
void printWithColor(char * string, unsigned char colorCode);


// COLORES: El formato es COLORLETRA_COLORFONDO
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F
    
#endif