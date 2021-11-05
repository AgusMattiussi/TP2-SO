#include "strings.h"

char *answer = "JUGOSO";
char *guess = "______";
char remainingLettersCount = 6;

char lives = 10;

bool tryAddPlay(char letter) {
    letter = toUpperCase(letter);
    bool isCorrect = false;

    for (int i = 0; answer[i] != '\0'; i++) {
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