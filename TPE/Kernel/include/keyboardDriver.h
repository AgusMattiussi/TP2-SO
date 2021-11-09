#ifndef __keyboardDriver_H_
#define __keyboardDriver_H_

#include <naiveConsole.h>

extern int keyboardActivated();

extern unsigned char getPressedKey();

int keyboardHandler();

unsigned char scancodeToAscii(int scancode);

unsigned char * getBuffer();

unsigned char kb_getChar();

void removeFirstChar();

#endif