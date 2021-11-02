#include "stdio.h"

char getChar(){
    return sys_getChar();
}

void putChar(char c){
    sys_putChar(c);
}

void print(char * string){
    while (*string != 0){
        putChar(*string);
        *string++;
    }    
}