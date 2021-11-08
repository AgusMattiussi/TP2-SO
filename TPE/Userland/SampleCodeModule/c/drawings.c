#include <drawings.h>

static void drawSudokuGrid();

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
    printInPos("Press 'R' to play again", HANGMAN_CORRECT_WORD_ROW + 2, 5, BLACK_LIGHTGREEN);
}


void drawHangmanLostScreen(char * correctWord){
    for (int i = 0; i <= HANGMAN_LAST_COL; i++)
        drawVerticalLine(HANGMAN_FIRST_ROW, HANGMAN_LAST_ROW, i, RED_RED);

    printInPos("YOU LOST", HANGMAN_STATE_ROW, 11, WHITE_RED);
    printInPos("The word was:", HANGMAN_CORRECT_WORD_ROW - 2, 5, WHITE_RED);
    printInPos(correctWord, HANGMAN_CORRECT_WORD_ROW, 7, YELLOW_RED);
    printInPos("Press 'R' to play again", HANGMAN_CORRECT_WORD_ROW + 2, 5, WHITE_RED);
}

void clearHangmanScreen(){
    for (int i = 0; i <= HANGMAN_LAST_COL; i++)
        drawVerticalLine(HANGMAN_FIRST_ROW, HANGMAN_LAST_ROW, i, BLACK_BLACK);
}

void drawSudoku(){ 
    drawSudokuGrid();
}

static void drawSudokuGrid(){
    for (int i = SUDOKU_FIRST_COL; i <= SUDOKU_LAST_COL; i++){
        if((i - SUDOKU_FIRST_COL) % 4 == 0)
            drawVerticalLine(SUDOKU_FIRST_ROW, SUDOKU_LAST_ROW, i, BLUE_BLUE);
        else
            drawVerticalLine(SUDOKU_FIRST_ROW + 1, SUDOKU_LAST_ROW - 1, i, BLUE_WHITE);
    }   
    
    for (int i = SUDOKU_FIRST_ROW; i <= SUDOKU_LAST_ROW; i += 4){
            drawHorizontalLine(SUDOKU_FIRST_COL, SUDOKU_LAST_COL, i, BLUE_BLUE);
    }

    for (int i = SUDOKU_FIRST_COL + 1; i < SUDOKU_LAST_COL; i++){
        for (int j = SUDOKU_FIRST_ROW + 1; j < SUDOKU_LAST_ROW; j++){
            //Columna impar, fila par (distinta de 4)
            if(((i-SUDOKU_FIRST_COL) % 2 != 0) && ((j-SUDOKU_FIRST_ROW) % 4 == 2)){
                    printInPos(" ", j, i, BLUE_GREY);
            } 
            //Columna par (distinta de 4), fila impar
            else if(((i-SUDOKU_FIRST_COL) % 4 == 2) && ((j-SUDOKU_FIRST_ROW) % 2 != 0)){
                    printInPos(" ", j, i, BLUE_GREY);
            }
        }
    }
    
}

void redrawSudokuTiles(int sudoku[9][9]){
    char toPrint[2] = {'0'};
    int sudokuX = 0;
    int sudokuY = 0;

    int i;
    for (i = SUDOKU_FIRST_COL; i < SUDOKU_LAST_COL; i++){
        sudokuX = 0;
        for (int j = SUDOKU_FIRST_ROW; j < SUDOKU_LAST_ROW; j++){
            toPrint[0] = sudoku[sudokuX][sudokuY] != 0 ? (sudoku[sudokuX][sudokuY] + '0') : ' ';

            if((i-SUDOKU_FIRST_COL)%4 != 0 && (j-SUDOKU_FIRST_ROW)%4 != 0){
                //Columna impar
                if(((i-SUDOKU_FIRST_COL)%2 != 0)){
                    //Fila impar
                    if((j-SUDOKU_FIRST_ROW) % 2 == 1)
                        printInPos(toPrint, j, i, BLUE_WHITE);
                    //Fila par (distinta de 4)
                    else
                        printInPos(toPrint, j, i, BLUE_GREY);
                } 
                //Columna par (distinta de 4)
                else {
                    //Fila impar
                    if((j-SUDOKU_FIRST_ROW) % 2 != 0)
                        printInPos(toPrint, j, i, BLUE_GREY);
                    //Fila par (distinta de 4)
                    else
                        printInPos(toPrint, j, i, BLUE_WHITE);
                }
                sudokuX++;
            }
           
        }
        if((i-SUDOKU_FIRST_COL)%4 != 0)
            sudokuY++;
    }
}

void drawSudokuWinScreen(){
    for (int i = SUDOKU_SECTION_FIRST_COL; i < SUDOKU_SECTION_LAST_COL; i++){
        drawVerticalLine(0, 24, i, LIGHTGREEN_LIGHTGREEN);
    }

    printInPos("YOU WON", 12, SUDOKU_SECTION_LAST_COL - SUDOKU_SECTION_FIRST_COL + 1, BLUE_LIGHTGREEN);
    printInPos("Press '0' to play again", 15, SUDOKU_SECTION_LAST_COL - SUDOKU_SECTION_FIRST_COL - 5, BLACK_LIGHTGREEN);

    
}

