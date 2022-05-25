#ifndef _BUDDY_H
#define _BUDDY_H

// #include <stdint.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>

#include <types.h>
#include <stdint.h>

void * malloc(size_t request);
void free(void *ptr);

#endif