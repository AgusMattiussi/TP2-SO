#ifndef _SUDOKU_HANDLER_H_
#define _SUDOKU_HANDLER_H_

#include <sudoku.h>
#include <drawings.h>
#include <stdio.h>

#define TABLE_UPPER_LEFT_CORNER_POS_X 42
#define TABLE_UPPER_LEFT_CORNER_POS_Y 1
#define INSTRUCTION_TEXT_COLOR_CODE CYAN_BLACK

void startSudoku();
void updateSudoku(char digit);

#endif