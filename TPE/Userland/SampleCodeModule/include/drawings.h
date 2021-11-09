#ifndef _DRAWINGS_H_
#define _DRAWINGS_H_

#include <stdio.h>

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

/**
 * Imprime el caracter ' ' con el color especificado por parametro (colorCode) 
 * a lo largo de una columna (col) desde una fila (fromRow) hasta otra (toRow)
**/
void drawVerticalLine(int fromRow, int toRow, int col, unsigned char colorCode);

/**
 * Imprime el caracter ' ' con el color especificado por parametro (colorCode) 
 * a lo largo de una fila (row) desde una columna (fromCol) hasta otra (toCol)
**/
void drawHorizontalLine(int fromCol, int toCol, int row, unsigned char colorCode);

/**
 * Divide la pantalla en 4 zonas divididas por lineas rojas para el display del gamemode
**/
void drawGameMode();

/**
 * Muestran una pantalla con un mensaje de victoria o derrota respectivamente, 
 * la palabra a adivinar y las instrucciones por si se quiere volver a jugar
**/
void drawHangmanWinningScreen(char * correctWord);
void drawHangmanLostScreen(char * correctWord);

/**
 * Limpia la pantalla con el mensaje de juego terminado para volver a jugar
**/
void clearHangmanScreen();

/**
 * Dibuja el sudoku en pantalla, con su grid y los numeros correspondientes
**/
void drawSudoku(char sudoku[9][9]);

/**
 * Muestran una pantalla con un mensaje de victoria cuando logro completar el sudoku 
**/
void drawSudokuWinScreen();

/**
 * Recibe una columna (col), una fila (row) y un digito (digit) por parametro y se encarga
 * de actualizar esa celda del sudoku
**/
void drawInSudokuPos(int col, int row, char digit);

#endif