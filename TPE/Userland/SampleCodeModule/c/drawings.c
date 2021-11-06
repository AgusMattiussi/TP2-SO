#include <drawings.h>

void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode){
    if(fromRow > toRow)
        return;

    for (int i = fromRow; i <= toRow; i++){
        printInPos(" ", i, col, colorCode);
    }
    
}

void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode){
    if(fromCol > toCol)
        return;
      
    for (int i = fromCol; i <= toCol; i++){
        printInPos(" ", row, i, colorCode);
    }
    
}

void drawGameMode(){
    drawVerticalLine(0, 24, 30, RED_RED);
    drawHorizontalLine(0, 30, 3, RED_RED);
    drawHorizontalLine(0, 30, 10, RED_RED);
}

void drawHangmanWinningScreen(char * correctWord){
    for (int i = 0; i <= HANGMAN_LAST_COL; i++)
        drawVerticalLine(HANGMAN_FIRST_ROW, HANGMAN_LAST_ROW, i, LIGHTGREEN_LIGHTGREEN);

    printInPos("YOU WON", HANGMAN_STATE_ROW, 11, BLUE_LIGHTGREEN);
    printInPos("The word was:", HANGMAN_CORRECT_WORD_ROW - 2, 5, BLACK_LIGHTGREEN);
    printInPos(correctWord, HANGMAN_CORRECT_WORD_ROW, 7, BLUE_LIGHTGREEN);
}


void drawHangmanLostScreen(char * correctWord){
    for (int i = 0; i <= HANGMAN_LAST_COL; i++)
        drawVerticalLine(HANGMAN_FIRST_ROW, HANGMAN_LAST_ROW, i, RED_RED);

    printInPos("YOU LOST", HANGMAN_STATE_ROW, 11, WHITE_RED);
    printInPos("The word was:", HANGMAN_CORRECT_WORD_ROW - 2, 5, WHITE_RED);
    printInPos(correctWord, HANGMAN_CORRECT_WORD_ROW, 7, YELLOW_RED);
}

void clearHangmanScreen(){
    for (int i = 0; i <= HANGMAN_LAST_COL; i++)
        drawVerticalLine(HANGMAN_FIRST_ROW, HANGMAN_LAST_ROW, i, BLACK_BLACK);
}