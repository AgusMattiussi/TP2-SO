#ifndef SYSCALL_DISPATCHER_H
#define SYSCALL_DISPATCHER_H

#include <stdint.h>
#include <keyboardDriver.h>
#include <rtcDriver.h>
#include <naiveConsole.h> //SACAR ESTO

extern void getRegistersInfo();

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif