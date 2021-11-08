#ifndef _SUDOKU_H
#define _SUDOKU_H

#define TABLE_SIZE 3
#define TABLE_TOTAL_SIZE (TABLE_SIZE * TABLE_SIZE)

int tryAddPlayForSudoku(char number, int rowIndex, int columnIndex);
char *getRowHighlightedNumbersPos(int row);

#endif