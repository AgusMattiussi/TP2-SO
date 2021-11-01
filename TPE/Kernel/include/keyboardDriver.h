#ifndef __keyboardDriver_H_
#define __keyboardDriver_H_

extern int keyboardActivated();

extern unsigned char getPressedKey();

int keyboardHandler();

char scancodeToAscii(int scancode);

char * getBuffer();

#endif