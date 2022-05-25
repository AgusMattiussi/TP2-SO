#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include <types.h>
#include <stdint.h>

void * malloc(size_t wantedSize);
void free(void * p);

#endif