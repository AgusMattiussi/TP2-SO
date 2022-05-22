#ifndef _BUDDY_H
#define _BUDDY_H

// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>

#include <types.h>


void * b_malloc(size_t request);

void b_free(void *ptr);

#endif