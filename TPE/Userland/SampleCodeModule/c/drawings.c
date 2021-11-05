#include <drawings.h>

void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode){
    if(fromRow > toRow)
        return;

    for (int i = fromRow; i <= toRow; i++){
        sys_printInPos(" ", i, col, colorCode);
    }
    
}

void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode){
    if(fromCol > toCol)
        return;
      
    for (int i = fromCol; i <= toCol; i++){
        sys_printInPos(" ", row, i, colorCode);
    }
    
}

void drawGameMode(){
    drawVerticalLine(0, 24, 30, RED_RED);
    drawHorizontalLine(0, 30, 5, RED_RED);
    drawHorizontalLine(0, 30, 10, RED_RED);
}