#ifndef _SUDOKU_HANDLER_H_
#define _SUDOKU_HANDLER_H_

#include <sudoku.h>
#include <drawings.h>
#include <stdio.h>

/// POSICIÓN Y CÓDIGO DE COLOR: estos campos indican la posición del Sudoku en la pantalla
/// y el color para las instucciones de columna y fila de inserción.
#define TABLE_UPPER_LEFT_CORNER_POS_X 42
#define TABLE_UPPER_LEFT_CORNER_POS_Y 1
#define INSTRUCTION_TEXT_COLOR_CODE CYAN_BLACK

/// Resetea el Sudoku y dibuja el Sudoku inical en pantalla.
void startSudoku();

/// Usa un dígito ingresado por teclado para tomar la columna, luego la fila y por último el
/// número. Luego lo inserta y redibuja las partes de la interfaz del Sudoku que deban
/// modificarse.
void updateSudoku(char digit);

#endif