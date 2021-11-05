#ifndef __keyboardDriver_H_
#define __keyboardDriver_H_

extern int keyboardActivated();

extern unsigned char getPressedKey();

int keyboardHandler();

unsigned char scancodeToAscii(int scancode);

char * getBuffer();

char kb_getChar();

void removeFirstChar();

#endif