#include <gamemode.h>

static void updateTime();
static void updateChronometer(unsigned char c);
static void hangmanHandler(char c);
static void printInTimeSector(int hours, int minutes, int seconds);
static void printInChronoSector(int hours, int minutes, int seconds, int milis);
static void printCurrentChronoTime(unsigned long decisecondsElapsed);
static void restartChronoTime();
static void updateChronoStateMessage();

enum chronoStates{STOPPED, PAUSED, RUNNING};

int chronoState = STOPPED;
int hangmanState = PLAYING;
unsigned long startDeciseconds = 0;



void gamemodeManager(){
    unsigned char c;
    restartChronoTime();
    updateChronoStateMessage();
    startHangman();

    //PARA PROBAR EL SUDOKU
    // int sudoku[9][9] = { 
    //     {1, 2, 3, 1, 2, 3, 1, 2, 3},
    //     { 4, 5, 6, 4, 5, 6, 4, 5, 6},
    //     { 7, 8, 9, 7, 8, 9, 7, 8, 9},
    //     {1, 2, 3, 1, 2, 3, 1, 2, 3}, 
    //     { 4, 5, 6, 4, 5, 6, 4, 5, 6}, 
    //     { 7, 8, 9, 7, 8, 9, 7, 8, 9}, 
    //     {1, 2, 3, 1, 2, 3, 1, 2, 3}, 
    //     { 4, 5, 6, 4, 5, 6, 4, 5, 6}, 
    //     { 7, 8, 9, 7, 8, 9, 7, 8, 9}
    // };

    int sudoku2[9][9] = { 
        {1, 0, 3, 1, 2, 3, 1, 2, 3},
        { 4, 0, 6, 4, 5, 0, 0, 5, 6},
        { 7, 8, 9, 7, 8, 0, 7, 0, 0},
        {1, 2, 0, 1, 2, 0, 1, 2, 3}, 
        { 4, 5, 6, 4, 0, 6, 4, 0, 6}, 
        { 7, 0, 9, 0, 0, 9, 0, 8, 9}, 
        {1, 0, 3, 1, 2, 3, 1, 2, 3}, 
        { 0, 5, 6, 0, 5, 6, 4, 5, 6}, 
        { 7, 0, 0, 7, 0, 9, 0, 8, 9}
    };

    drawSudoku(sudoku2);
    drawInSudokuPos(3, 1, '9');

    while((c = getCharOrNull()) != ESCAPE_KEY){
        updateTime();
        updateChronometer(c);
        
        if(IS_ALPHA(c))
            hangmanHandler(c);
        //else if(IS_DIGIT(c))
            //SUDOKUHANLDER
    }
    clear();    
}

static void updateTime(){
    int hours = (getTime(HOURS) - 3) % 24;
    int minutes = getTime(MINUTES);
    int seconds = getTime(SECONDS);

    printInTimeSector(hours, minutes, seconds);
}

static void printInTimeSector(int hours, int minutes, int seconds){
    char buffer[3];

    itoa(hours, buffer);
    printInPos(buffer, TIME_ROW, TIME_HOURS_COL, ORANGE_BLACK);

    printInPos(":", TIME_ROW, TIME_HOURS_COL + 2, ORANGE_BLACK);

    itoa(minutes, buffer);
    printInPos(buffer, TIME_ROW, TIME_MINUTES_COL, ORANGE_BLACK);

    printInPos(":", TIME_ROW, TIME_MINUTES_COL + 2, ORANGE_BLACK);

    itoa(seconds, buffer);
    printInPos(buffer, TIME_ROW, TIME_SECONDS_COL, ORANGE_BLACK);
}

static void updateChronometer(unsigned char c){
    static unsigned long currentDeciseconds;
    static unsigned long pausedDeciseconds;
    static unsigned long decisecondsWhenPaused;

    if(chronoState == RUNNING){
        currentDeciseconds = decisecondsWhenPaused + (getDeciseconds() - startDeciseconds) - pausedDeciseconds;
        printCurrentChronoTime(currentDeciseconds);
    } else if(chronoState == PAUSED)
        pausedDeciseconds = getDeciseconds() - startDeciseconds;

    if(c == ALT_KEY){
        chronoState = STOPPED;
        startDeciseconds = 0;
        currentDeciseconds = 0;
        restartChronoTime();
        updateChronoStateMessage();
    } else if(c == CONTROL_KEY){
        switch (chronoState) {
        case STOPPED:
            chronoState = RUNNING;
            currentDeciseconds = 0;
            pausedDeciseconds = 0;
            decisecondsWhenPaused = 0;
            startDeciseconds = getDeciseconds();
            updateChronoStateMessage();
            break;
        case RUNNING:
            chronoState = PAUSED;
            decisecondsWhenPaused = currentDeciseconds;
            updateChronoStateMessage();
            break;
        case PAUSED:
            chronoState = RUNNING;
            updateChronoStateMessage();
            break;
        }
    }
}

static void printCurrentChronoTime(unsigned long decisecondsElapsed){
    int deciseconds = decisecondsElapsed % 10;
    int seconds = (decisecondsElapsed /= 10) % 60;
    int minutes = (decisecondsElapsed /= 60) % 60;
    int hours = (decisecondsElapsed /= 60) % 24;
    printInChronoSector(hours, minutes, seconds, deciseconds);
}

static void restartChronoTime(){
    printInChronoSector(0, 0, 0, 0);
}

static void printInChronoSector(int hours, int minutes, int seconds, int decisecs){
    char buffer[3];

    itoa(hours, buffer);
    printInPos(buffer, CHRONO_ROW, TIME_HOURS_COL, WHITE_BLACK);

    printInPos(":", CHRONO_ROW, TIME_HOURS_COL + 2, WHITE_BLACK);

    itoa(minutes, buffer);
    printInPos(buffer, CHRONO_ROW, TIME_MINUTES_COL, WHITE_BLACK);

    printInPos(":", CHRONO_ROW, TIME_MINUTES_COL + 2, WHITE_BLACK);

    itoa(seconds, buffer);
    printInPos(buffer, CHRONO_ROW, TIME_SECONDS_COL, WHITE_BLACK);

    printInPos(".", CHRONO_ROW, TIME_SECONDS_COL + 2, WHITE_BLACK);

    char decisecsBuffer[2] = {(char)decisecs + '0', 0};
    printInPos(decisecsBuffer, CHRONO_ROW, CHRONO_DECISECONDS_COL, WHITE_BLACK);
}

static void updateChronoStateMessage(){
    char * message;
    unsigned char color;

    switch (chronoState){
        case STOPPED:
            message = "STOPPED";
            color = RED_BLACK;
            break;
        case PAUSED:
            message = "PAUSED ";
            color = CYAN_BLACK;
            break;
        case RUNNING:
            message = "RUNNING";
            color = GREEN_BLACK;
    }

    printInPos(message, CHRONO_MSG_ROW, TIME_HOURS_COL + 2, color);
}

static void hangmanHandler(char c){
    if(hangmanState != PLAYING && c == 'r'){
        clearHangmanScreen();
        hangmanState = PLAYING;
        startHangman();
        return;
    }

    if(hangmanState == PLAYING){
        hangmanState = updateHangman(c);

        if(hangmanState == WON)
            drawHangmanWinningScreen(getCurrentWord());
        else if(hangmanState == LOST)
            drawHangmanLostScreen(getCurrentWord());
    }
}


