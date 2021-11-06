#ifndef _HANGMAN_H
#define _HANGMAN_H

#include <strings.h>

#define STARTING_LIVES 7
#define WORD_ARRAY_SIZE 3
#define LETTER_COUNT 'Z' - 'A' + 1
#define MAX_WORD_LENGTH 30

#define LIVES_ROW 13
#define LIVES_COL 9
#define LIVES_COL2 16

#define GUESSED_ROW 15
#define ALREADY_CHOSEN_ROW 17
#define ALREADY_CHOSEN_COL 2


int updateHangman(char c);
int tryAddPlayForHangman(char letter);
void startHangman();

#endif