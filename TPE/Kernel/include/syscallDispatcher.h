#ifndef SYSCALL_DISPATCHER_H
#define SYSCALL_DISPATCHER_H

#include <stdint.h>
#include <keyboardDriver.h>

uint64_t syscallDispatcher(uint64_t rax);

#endif