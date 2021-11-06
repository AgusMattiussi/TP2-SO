#ifndef _HANGMAN_H
#define _HANGMAN_H

#include <strings.h>

#define STARTING_LIVES 7
#define WORD_ARRAY_SIZE 3
#define LETTER_COUNT 'Z' - 'A' + 1

int updateHangman(char c);
int tryAddPlayForHangman(char letter);
void startHangman();

#endif