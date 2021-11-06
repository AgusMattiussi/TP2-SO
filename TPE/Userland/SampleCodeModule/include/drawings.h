#ifndef _DRAWINGS_H_
#define _DRAWINGS_H_

#include <stdio.h>

void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode);
void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode);
void drawGameMode();
void drawHangmanWinningScreen(char * correctWord);
void drawHangmanLostScreen(char * correctWord);
void clearHangmanScreen();

#define HANGMAN_LAST_COL 29
#define HANGMAN_FIRST_ROW 11
#define HANGMAN_LAST_ROW 24
#define HANGMAN_STATE_ROW 14
#define HANGMAN_CORRECT_WORD_ROW HANGMAN_STATE_ROW + 5

#endif