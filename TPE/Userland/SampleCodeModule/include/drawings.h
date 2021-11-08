#ifndef _DRAWINGS_H_
#define _DRAWINGS_H_

#include <stdio.h>

void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode);
void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode);
void drawGameMode();
void drawHangmanWinningScreen(char * correctWord);
void drawHangmanLostScreen(char * correctWord);
void clearHangmanScreen();
void drawSudoku();
void redrawSudokuTiles(int sudoku[9][9]);
void drawSudokuWinScreen();

#define HANGMAN_LAST_COL 29
#define HANGMAN_FIRST_ROW 11
#define HANGMAN_LAST_ROW 24
#define HANGMAN_STATE_ROW 14
#define HANGMAN_CORRECT_WORD_ROW HANGMAN_STATE_ROW + 5
#define SUDOKU_FIRST_COL 50
#define SUDOKU_LAST_COL SUDOKU_FIRST_COL + 4*3
#define SUDOKU_FIRST_ROW 5
#define SUDOKU_LAST_ROW SUDOKU_FIRST_ROW + 4*3
#define SUDOKU_SECTION_FIRST_COL 31
#define SUDOKU_SECTION_LAST_COL 80

#endif