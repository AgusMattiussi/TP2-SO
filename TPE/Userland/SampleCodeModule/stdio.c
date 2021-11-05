#include <stdio.h>


#define SOLID_CHAR 219; // <-- █ 
//#define SOLID_CHAR 178;

char getChar(){
    char c;
    while((c = sys_getChar()) == 0);
    return c;
}

char getCharOrNull(){
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

void printInPos(char * string, int row, int col, unsigned char colorCode){
    sys_printInPos(string, row, col, colorCode);
}

void printVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode){
    if(fromRow > toRow)
        return;

    
    for (int i = fromRow; i <= toRow; i++){
        sys_printInPos(" ", i, col, colorCode);
    }
    
}

void printHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode){
    if(fromCol > toCol)
        return;
    
        
    for (int i = fromCol; i <= toCol; i++){
        sys_printInPos(" ", row, i, colorCode);
    }
    
}

int scan(char *buffer, int size){
    char c;
    int lenght=0;
    while((c = getChar()) != '\n' && lenght < size){
        if(c == BACKSPACE){
            if(lenght > 0)
                buffer[lenght--] = 0;
        } else {
            putChar(c);
            buffer[lenght++] = c;
        }
    }
    buffer[lenght] = 0;
    return lenght;
}

void clear(){
    sys_clear();
}