#include <stdio.h>

char getChar(){
    return sys_getChar();
}

void putChar(char c){
    sys_putCharWC(c, WHITE_BLACK);
}

void putCharWithColor(char c, unsigned char colorCode){
    sys_putCharWC(c, colorCode);
}

void print(char * string){
    while (*string != 0){
        putChar(*string);
        string++;
    }    
}