#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include <stdio.h> 
#include <strings.h> 
#include <hangman.h>

void gamemodeManager();

#define ESCAPE_KEY 0x1B
#define CONTROL_KEY 200
#define ALT_KEY 202
#define SECONDS 0
#define MINUTES 2
#define HOURS 4
#define TIME_ROW 1
#define TIME_HOURS_COL 11
#define TIME_MINUTES_COL 14
#define TIME_SECONDS_COL 17
#define CHRONO_ROW 5
#define CHRONO_DECISECONDS_COL 20
#define CHRONO_MSG_ROW CHRONO_ROW + 2
#define HANGMAN_RESET_KEY 'r'

#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z') 
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

#endif