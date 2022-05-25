#include <rtcDriver.h>

uint8_t getTime(uint64_t descriptor){
    return getRTC(descriptor);
}

uint64_t getDecis(){
    return deciseconds_elapsed();
}

uint64_t getSeconds(){
    return seconds_elapsed();
}