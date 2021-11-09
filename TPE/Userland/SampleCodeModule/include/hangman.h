#ifndef _HANGMAN_H
#define _HANGMAN_H

#include <strings.h>

#define STARTING_LIVES 7
#define WORD_ARRAY_SIZE 3
#define LETTER_COUNT 'Z' - 'A' + 1
#define MAX_WORD_LENGTH 30

#define LIVES_ROW 13
#define LIVES_COL 9
#define LIVES_COL2 16

#define GUESSED_ROW 15
#define ALREADY_CHOSEN_ROW 17
#define ALREADY_CHOSEN_COL 2

/**
 * Inicializa el juego de ahorcado
**/
void startHangman();

/**
 * Actualiza el ahorcado segun el resultado de la última jugada
 * Si la jugada fue invalida no hace nada
 * Si el intento fue correcto actualiza el juego con la letra ingresada
 * Si la letra ingresada no correspondia a la palabra disminuye una vida
 * Finalmente imprime las letras ya utilizadas anteriormente
**/
int updateHangman(char c);

/**
 * Maneja la lógica del juego de ahorcado, recibe una letra (letter) por parametro,
 * Si ya fue utilizada retorna que la jugada fue invalida, sino chequea con la solución y
 * devuelve dos posibles estados GUESSED o FAILED
**/
int tryAddPlayForHangman(char letter);

/**
 * Retorna la palabra que tiene que adivinar el usuario
**/
char * getCurrentWord();

/**
 * Enums que manejan los estados correspondientes del ahorcado
**/
enum hangmanStates{LOST, PLAYING, WON};
enum guessResult{FAILED, ALREADY_CHOSEN, GUESSED};

#endif