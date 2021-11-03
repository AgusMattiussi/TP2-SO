#include <stdio.h>

char getChar(){
    // char c = sys_getChar();
    // if(c != 0)
    //     sys_putChar(c);
    // else
    //     sys_putChar('M');
    // return 'L';
    char c;
    while((c = sys_getChar()) == 0);
    return c;
}

void putChar(char c){
    if(c != 0)
        sys_putChar(c);
}

void print(char * string){
    while (*string != 0){
        putChar(*string);
        string++;
    }    
}

int scan(char *buffer, int size){
    char c;
    int lenght=0;
    while((c = getChar()) != '\n' && lenght < size){
        putChar(c);
        buffer[lenght++] = c;
    }
    buffer[lenght] = 0;
    return lenght;
}