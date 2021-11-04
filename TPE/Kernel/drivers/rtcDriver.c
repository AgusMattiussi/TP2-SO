#include <rtcDriver.h>

uint8_t getTime(uint64_t descriptor){
    return getRTC(descriptor);
}