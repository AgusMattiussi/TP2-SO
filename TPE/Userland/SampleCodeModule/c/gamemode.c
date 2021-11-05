#include <gamemode.h>

static void updateTime();
static void updateChronometer(char c);
static void updateGamesAndChrono(char c);
static void updateSudoku(int x);
static void updateHangman(char c);
static void updateGames(char c);
static void printInTimeSector(int hours, int minutes, int seconds);

static int isChronoRunning = 0;
static unsigned long startDeciseconds = 0;

void gamemodeManager(){
    char c;
    while((c = getCharOrNull()) != ESCAPE_KEY){
        updateTime();
        updateChronometer(c);
        
        if(IS_ALPHA(c) || IS_DIGIT(c))
            updateGames(c);
    }
    clear();    
}

static void updateTime(){
    int hours = getTime(HOURS);

    switch(hours){
        case 0:
            hours = 21;
        case 1:
            hours = 22;
        case 2:
            hours = 23;
        case 4:
            hours = 22;
        case 5:
            hours = 23;
        default:
            hours -= 3;
    }

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

static void updateChronometer(char c){
    if(startDeciseconds == 0)
        printInPos("00:00:00.0", 5, 11, CYAN_BLACK); // hh:mm:ss,d
    
    unsigned long currentDeciseconds = getDeciseconds();

    if(isChronoRunning){
        unsigned long decisecondsDiff = currentDeciseconds - startDeciseconds;
        int deciseconds = decisecondsDiff % 10;
        int seconds = (decisecondsDiff /= 10) % 60;
        int minutes = (decisecondsDiff /= 60) % 60;
        int hours = (decisecondsDiff /= 60) % 24;
        printInChronoSector(hours, minutes, seconds, deciseconds);
    }
    
    if(c == CONTROL_KEY)
        isChronoRunning = !isChronoRunning;
    if(c == ALT_KEY){
        isChronoRunning = 0;
        startDeciseconds = 0;
    }

}

static void updateGames(char c){
    if(IS_ALPHA(c))
        updateSudoku(c - '0');
    else
        updateHangman(c);
}

static void updateSudoku(int x){

}

static void updateHangman(char c){

}