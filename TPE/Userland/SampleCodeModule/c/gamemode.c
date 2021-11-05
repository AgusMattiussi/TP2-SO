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