#include <stdio.h>

unsigned char getChar(){
    char c;
    while((c = sys_getChar()) == 0);
    return c;
}

unsigned char getCharOrNull(){
    return sys_getChar();
}

void putCharWithColor(char c, unsigned char colorCode){
    if(c != 0)
        sys_putCharWC(c, colorCode);
}

void putChar(char c){
    if(c != 0)
        sys_putCharWC(c, WHITE_BLACK);
}

void print(char * string){
    while (*string != 0){
        putChar(*string);
        string++;
    }    
}

void printWithColor(char * string, unsigned char colorCode){
    while (*string != 0){
        putCharWithColor(*string, colorCode);
        string++;
    }    
}

void printCharInPos(char c, int row, int col, unsigned char colorCode){
    sys_printCharInPos(c, row, col, colorCode);
}

void printInPos(char * string, int row, int col, unsigned char colorCode){
    for (int i = 0; string[i] != 0; i++){
        printCharInPos(string[i], row, col + i, colorCode);
    }  
}

int scan(char *buffer, int size){
    char c;
    int lenght=0;
    while((c = getChar()) != '\n' && lenght < size){
        if(c == BACKSPACE){
            if(lenght > 0)
                buffer[lenght--] = 0;
        } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' '){
            putChar(c);
            buffer[lenght++] = c;  
        }      
    }
    while(buffer[lenght-1] == ' '){
        while((c = getChar()) != '\n' && lenght < size){
            if(c == BACKSPACE){
                if(lenght > 0)
                    buffer[lenght--] = 0;
            } else if(IS_ALPHA(c) || IS_DIGIT(c) || c == ' '){
                putChar(c);
                buffer[lenght++] = c;  
            }      
        }
    }
    buffer[lenght] = 0;
    return lenght;
}

void clear(){
    sys_clear();
}

int getTime(int descriptor){
    return sys_getTime(descriptor);
}

unsigned long getDeciseconds(){
    return sys_getDeciseconds();
}
