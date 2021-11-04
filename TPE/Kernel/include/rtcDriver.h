#ifndef __RTCDriver_H_
#define __RTCDriver_H_
#include <stdint.h>

extern uint8_t getRTC(uint64_t descriptor);

uint8_t getTime(uint64_t descriptor);

#endif