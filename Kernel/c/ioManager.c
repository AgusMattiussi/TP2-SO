#include <ioManager.h>

void print(char * str){
    printWithColor(str, WHITE_BLACK);
}

void printWithColor(char * str, uint8_t colorCode){
    writeStrHandler(getFdOut(), str, colorCode);
}

void putChar(char c){
    putCharWithColor(c, WHITE_BLACK);
}

void putCharWithColor(char c, uint8_t colorCode){
    writeCharHandler(getFdOut(), c, colorCode);
}

uint32_t readHandler(int fd){
    if(fd == STDIN)
        return kb_getChar();
    return readPipeWithFd(fd);  
}

uint32_t writeStrHandler(int fd, char * str, uint8_t colorCode){
    if(fd == STDOUT){
        ncPrintWithColor(str, colorCode);
        return strlen(str);
    }
    else
        return writeInPipeWithFd(fd, str);
}

uint32_t writeCharHandler(int fd, char c, uint8_t colorCode){
    if(fd == STDOUT){
        ncPrintCharWithColor(c, colorCode);
        return 1;
    }
    else
        return writeCharInPipeWithFd(fd, c);
}