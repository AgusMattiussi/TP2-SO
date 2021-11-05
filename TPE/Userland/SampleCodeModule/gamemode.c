#include <gamemode.h>

static void updateTime();
static void updateChronometer(char c);
static void updateGamesAndChrono(char c);
static void updateSudoku(int x);
static void updateHangman(char c);
static void updateGames(char c);

static int isChronoRunning = 0;

void gamemodeManager(){
    char c;
    while((c = getCharOrNull()) != ESCAPE_KEY){
        updateTime();
        updateChronometer(c);
        
        if(IS_ALPHA(c) || IS_DIGIT(c))
            updateGames(c);
    }    
}

static void updateTime(){
    char str[3];
    int hours = sys_getTime(HOURS);

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

    itoa(hours, str);
    printInPos(str, 3, 3, ORANGE_BLACK);

    printInPos(":", 3, 5, ORANGE_BLACK);

    itoa(sys_getTime(MINUTES), str);
    printInPos(str, 3, 6, ORANGE_BLACK);

    printInPos(":", 3, 8, ORANGE_BLACK);


    itoa(sys_getTime(SECONDS), str);
    printInPos(str, 3, 9, ORANGE_BLACK);

}

static void updateChronometer(char c){
    if(isChronoRunning)
        //printInChronoSpace(newTime());
    if(c == CONTROL_KEY)
        isChronoRunning = !isChronoRunning;
    if(c == ALT_KEY){
        isChronoRunning = 0;
        //printInChronoSpace("         ");
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