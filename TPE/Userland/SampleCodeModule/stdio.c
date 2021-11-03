#include <stdio.h>

char getChar(){
    // char c = sys_getChar();
    // if(c != 0)
    //     sys_putChar(c);
    // else
    //     sys_putChar('M');
    // return 'L';

    return sys_getChar();
}

void putChar(char c){
    // if(c == 0)
    //     sys_putChar('D');
    sys_putChar(c);
}

void print(char * string){
    while (*string != 0){
        putChar(*string);
        string++;
    }    
}