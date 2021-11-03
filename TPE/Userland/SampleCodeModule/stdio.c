#include <stdio.h>

char getChar(){
    char c;
    while((c = sys_getChar()) == 0);
    return c;
}

void putChar(char c){
    sys_putCharWC(c, WHITE_BLACK);
}

void putCharWithColor(char c, unsigned char colorCode){
    sys_putCharWC(c, colorCode);
  
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