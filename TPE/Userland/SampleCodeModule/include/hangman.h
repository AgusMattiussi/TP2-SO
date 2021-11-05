#ifndef _HANGMAN_H
#define _HANGMAN_H

#define WORD_LENGTH 6
#define STARTING_LIVES 10

#include <strings.h>

int tryAddPlay(char letter);
char getRemainingLives();

#endif