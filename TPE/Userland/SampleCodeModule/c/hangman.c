#include <hangman.h>
#include <stdio.h>

static char * words[WORD_ARRAY_SIZE] = {"JUGOSO", "ARQUITECTURA", "PEDRO"};
static int wordLengths[WORD_ARRAY_SIZE] = {6, 12, 5};

static int getRandomWordIndex();

int lives = STARTING_LIVES;
char * currentWord;
int currentWordLength;
// static char * guessed;
static char guessed[MAX_WORD_LENGTH] = {0};
char alreadyChosen[LETTER_COUNT] = {0};
int remainingLetters;

static void printLives();
static void printGuessed();
static void printAlreadyChosen();


void startHangman(){
    int wordIndex = getRandomWordIndex();

    currentWord = words[wordIndex];                     //Hacerlo escalable
    currentWordLength = wordLengths[wordIndex];
    
    remainingLetters = currentWordLength;

    // char aux[currentWordLength];
    // for (int i = 0; i < currentWordLength; i++)
    //     aux[i] = '_';

    // static char guessed[currentWordLength];
    for (int i = 0; i < currentWordLength; i++)
        guessed[i] = '_';

    printLives();
    printGuessed();
    printAlreadyChosen();
}

int updateHangman(char c){
    if(!tryAddPlayForHangman(convertCharToUpperCase(c)))
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
        return 0;
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
            return 1;
        }
    }
    return 0;
}

char * getCurrentWord(){
    return currentWord;
}

// "Random" ;) ;)
static int getRandomWordIndex(){
    static int idx = -1;
    idx++;
    return idx % WORD_ARRAY_SIZE; 
}


static void printLives(){
    printInPos("Lives: ", LIVES_ROW, LIVES_COL, GREEN_BLACK);
    char buff[2] = {(char)lives + '0', 0};
    printInPos(buff, LIVES_ROW, LIVES_COL2, GREEN_BLACK);
}

static void printGuessed(){
    printInPos(guessed, GUESSED_ROW, LIVES_COL, GREEN_BLACK);
    // for(int i=0; i<currentWordLength; i++){
    //     printCharInPos(guessed[i], GUESSED_ROW, LIVES_COL + 2*i, GREEN_BLACK);
    //     printCharInPos(' ', GUESSED_ROW, LIVES_COL + 2*i + i, GREEN_BLACK);
    // }
}

static void printAlreadyChosen(){
    printInPos("Already chosen letters:", ALREADY_CHOSEN_ROW, ALREADY_CHOSEN_COL, GREEN_BLACK);
    for(int i=0; i<LETTER_COUNT; i++){
        if(alreadyChosen[i]){
            char buff[2] = {i + 'A', 0};
            printInPos(buff, ALREADY_CHOSEN_ROW + 1, ALREADY_CHOSEN_COL + i, GREEN_BLACK);
        }
    }
    
}
