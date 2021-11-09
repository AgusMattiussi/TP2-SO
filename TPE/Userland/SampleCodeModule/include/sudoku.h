#ifndef _SUDOKU_H
#define _SUDOKU_H

#include <stdio.h>

/// TAMAÑOS: Estos campos guardan las dimensiones del Sudoku.
#define TABLE_SIZE 3
#define TABLE_TOTAL_SIZE (TABLE_SIZE * TABLE_SIZE)

/// Inicializa todas los campos del Sudoku.
void initializeSudoku();

/// Intenta colocar un número en una posición del Sudoku.
/// 
/// Devuelve 0 si no es una inserción válida, 1 si insertó el número y continúa con espacios
/// de números restantes, ó 2 si ganó.
int tryAddPlayForSudoku(char number, int rowIndex, int columnIndex);

/// Retorna una matriz de las dimensiones totales del Sudoku con los números iniciales.
/// 
/// Si una posición es inicialmente vacía, le coloca un 0.
char **getStartingNumbers();

enum sudokuAdditionResult{IGNORED, ADDED_AND_CAN_CONTINUE, ADDED_AND_WON};

#endif