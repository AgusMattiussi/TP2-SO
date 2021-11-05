#include <hangman.h>

char *answer = "J U G O S O";
char *guess = "_ _ _ _ _ _";
char remainingLettersCount = WORD_LENGTH;

char lives = STARTING_LIVES;

bool tryAddPlay(char letter) {
    letter = convertCharToUpperCase(letter);
    bool isCorrect = false;

    for (int i = 0; i < WORD_LENGTH * 2; i += 2) {
        if (guess[i] == letter) return false;
        if (answer[i] == letter) {
            isCorrect = true;
            guess[i] = letter;
            remainingLettersCount--;
        }
    }

    if (!isCorrect) return --lives == 0;
    return remainingLettersCount == 0;
}

char getRemainingLives() {
    return lives - 1;
}