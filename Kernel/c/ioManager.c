#include <ioManager.h>

uint32_t readHandler(int fd){
    if(fd == STDIN)
        return kb_getChar();
    return readPipeWithFd(fd);  
}

uint32_t writeHandler(int fd, char * str){
    if(fd == STDOUT){
        ncPrint(str);
        return strlen(str);
    }
    else
        return writeInPipeWithFd(fd, str);
}