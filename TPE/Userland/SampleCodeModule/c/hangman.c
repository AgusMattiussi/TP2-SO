#include <hangman.h>
#include <stdio.h>

static char * words[WORD_ARRAY_SIZE] = {"JUGOSO", "ARQUITECTURA", "PEDRO"};
static int wordLengths[WORD_ARRAY_SIZE] = {6, 12, 5};

enum hangmanStates{LOST, PLAYING, WON};

char lives = STARTING_LIVES;
char * currentWord;
int currentWordLength;
char * guessed;
char alreadyChosen[LETTER_COUNT] = {0};
int remainingLetters;

void startHangman(){
    currentWord = words[0];                     //Hacerlo escalable
    currentWordLength = wordLengths[0];
    
    remainingLetters = currentWordLength;

    char aux[currentWordLength];
    for (int i = 0; i < currentWordLength; i++)
        aux[i] = '_';
    guessed = aux;
}

int updateHangman(char c){
    if(!tryAddPlayForHangman(c))
        lives--;

    //printLives(lives);
    //printGuessed();
    //printArleadyChosen();

    if(lives == 0)
        return LOST;
    if(remainingLetters == 0)
        return WON;
    return PLAYING;
}
        
int tryAddPlayForHangman(char letter) {
    for (int i = 0; i < currentWordLength; i++) {
        //Ya elegida anteriormente
        if (alreadyChosen[letter - 'A']) 
            return 0;
        else
            alreadyChosen[letter - 'A'] = 1;
        //Si encuentro al menos un match
        if (currentWord[i] == letter) {
            for (int j = i; j < currentWordLength; j++){
                if(currentWord[j] == letter){
                    guessed[j] = letter;
                    remainingLetters--;
                }
            }
            return 1;
        }
    }
    return 0;
}
