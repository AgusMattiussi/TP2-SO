#include <gamemode.h>

static void updateTime();
static void updateChronometer(unsigned char c);
static void updateGames(char c);
static void printInTimeSector(int hours, int minutes, int seconds);
static void printInChronoSector(int hours, int minutes, int seconds, int milis);

static int isChronoRunning = 0;
static unsigned long startDeciseconds = 0;


void gamemodeManager(){
    unsigned char c;
    unsigned char buff[2];
    while((c = getCharOrNull()) != ESCAPE_KEY){
        updateTime();
        itoa(isChronoRunning, buff);
        //printInPos( buff, 11, 11, CYAN_BLACK);

        updateChronometer(c);
        
        if (IS_DIGIT(c)) {
            updateSudoku(c);
        } else if (IS_ALPHA(c)) {
            updateHangman(c);
        }
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
    if(startDeciseconds == 0)
        printInChronoSector(0, 0, 0, 0);
    char buff[] = {c, 0};
    printInPos(buff, 15, 11, CYAN_BLACK);
    //unsigned long currentDeciseconds = getDeciseconds();
    unsigned long currentDeciseconds = 0;

    
    if(isChronoRunning){
        unsigned long decisecondsDiff = currentDeciseconds - startDeciseconds;
        int deciseconds = decisecondsDiff % 10;
        int seconds = (decisecondsDiff /= 10) % 60;
        int minutes = (decisecondsDiff /= 60) % 60;
        int hours = (decisecondsDiff /= 60) % 24;
        printInChronoSector(hours, minutes, seconds, deciseconds);
    }
    
    if(c == 'f'){
        isChronoRunning = !isChronoRunning;
        printInPos("penis", 15, 11, CYAN_BLACK);
    }
    if(c == ALT_KEY){
        isChronoRunning = 0;
        startDeciseconds = 0;
    }else
        startDeciseconds = currentDeciseconds;

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
