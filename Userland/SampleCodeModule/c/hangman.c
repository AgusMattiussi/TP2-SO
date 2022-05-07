#include <hangman.h>
#include <stdio.h>

static char * words[WORD_ARRAY_SIZE] = {"JUGOSO", "ARQUITECTURA", "PEDRO"};
static int wordLengths[WORD_ARRAY_SIZE] = {6, 12, 5};

static int getRandomWordIndex();

int lives = STARTING_LIVES;
char * currentWord;
int currentWordLength;
static char guessed[MAX_WORD_LENGTH] = {0};
char alreadyChosen[LETTER_COUNT] = {0};
int remainingLetters;

static void printLives();
static void printGuessed();
static void printAlreadyChosen();


void startHangman(){
    lives = STARTING_LIVES;

    for (int i = 0; i < LETTER_COUNT; i++)
        alreadyChosen[i] = 0;

    int wordIndex = getRandomWordIndex();

    currentWord = words[wordIndex];                     
    currentWordLength = wordLengths[wordIndex];
    
    remainingLetters = currentWordLength;

    int i;
    for (i = 0; i < currentWordLength; i++)
        guessed[i] = '_';
    guessed[i] = 0;

    printLives();
    printGuessed();
    printAlreadyChosen();
}

int updateHangman(char c){

    if(tryAddPlayForHangman(convertCharToUpperCase(c)) == FAILED)
        lives--;

    printLives();
    printGuessed();
    printAlreadyChosen();

    if(lives == 0)
        return LOST;
    if(remainingLetters == 0)
        return WON;
    return PLAYING;
}
        
int tryAddPlayForHangman(char letter) {
    //Ya elegida anteriormente
    if (alreadyChosen[letter-'A']) 
        return ALREADY_CHOSEN;
    else
        alreadyChosen[letter-'A'] = 1;

    //La busco en currentWord
    for (int i = 0; i < currentWordLength; i++) {
        //Si encuentro al menos un match
        if (currentWord[i] == letter) {
            for (int j = i; j < currentWordLength; j++){
                if(currentWord[j] == letter){
                    guessed[j] = letter;
                    remainingLetters--;
                }
            }
            return GUESSED;
        }
    }
    return FAILED;
}

char * getCurrentWord(){
    return currentWord;
}

static int getRandomWordIndex(){
    static int idx = -1;
    idx++;
    return idx % WORD_ARRAY_SIZE; 
}


static void printLives(){
    printInPos("Lives: ", LIVES_ROW, LIVES_COL, GREEN_BLACK);
    printCharInPos((char)lives + '0', LIVES_ROW, LIVES_COL2, GREEN_BLACK);
}

static void printGuessed(){
    printInPos(guessed, GUESSED_ROW, LIVES_COL, GREEN_BLACK);
}

static void printAlreadyChosen(){
    printInPos("Already chosen letters:", ALREADY_CHOSEN_ROW, ALREADY_CHOSEN_COL, GREEN_BLACK);
    for(int i=0; i<LETTER_COUNT; i++){
        if(alreadyChosen[i]){
            printCharInPos(i + 'A', ALREADY_CHOSEN_ROW + 1, ALREADY_CHOSEN_COL + i, GREEN_BLACK);
        }
    }
    
}
