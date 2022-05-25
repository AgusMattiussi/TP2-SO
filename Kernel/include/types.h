#ifndef _TYPES_H_
#define _TYPES_H_

#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)

/* #ifndef _GCC_WRAP_STDINT_H
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned long long uint64_t;
#endif */

#include <stddef.h>

// typedef unsigned long long size_t;
typedef unsigned long long pid_t; 

#endif