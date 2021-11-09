#ifndef _SUDOKU_H
#define _SUDOKU_H

#include <stdio.h>

#define TABLE_SIZE 3
#define TABLE_TOTAL_SIZE (TABLE_SIZE * TABLE_SIZE)

void initializeSudoku();
int tryAddPlayForSudoku(char number, int rowIndex, int columnIndex);
char **getStartingNumbers();
char getNumberInPos(int row, int column);

#endif