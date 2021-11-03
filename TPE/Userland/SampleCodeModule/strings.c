#include <strings.h>

int strcmp(const char * s1, const char * s2){

    while (*s1 != 0){
        if(*s1 != *s2)
            return 1;
        s1++;
        s2++;
    }

    return !(*s2 == 0);
}

unsigned int strlen(const char * str){
    int count = 0;
    
    while (*str != 0){
        count++;
        str++;    
    }
    return count;
}