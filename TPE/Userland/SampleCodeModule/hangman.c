#include <hangman.h>

// char *answer = "J U G O S O";
// char *guess = "_ _ _ _ _ _";
// char remainingLettersCount = WORD_LENGTH;

// char lives = STARTING_LIVES;

// int tryAddPlay(char letter) {
//     letter = convertCharToUpperCase(letter);
//     int isCorrect = 0;

//     for (int i = 0; i < WORD_LENGTH * 2; i += 2) {
//         if (guess[i] == letter) return 0;
//         if (answer[i] == letter) {
//             isCorrect = 1;
//             guess[i] = letter;
//             remainingLettersCount--;
//         }
//     }

//     if (!isCorrect) return --lives == 0;
//     return remainingLettersCount == 0;
// }

// char getRemainingLives() {
//     return lives - 1;
// }