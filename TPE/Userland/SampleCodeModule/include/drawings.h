#ifndef _DRAWINGS_H_
#define _DRAWINGS_H_

#include <stdio.h>

void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode);
void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode);
void drawGameMode();

#endif