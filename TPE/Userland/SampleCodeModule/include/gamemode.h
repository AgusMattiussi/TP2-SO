#ifndef _GAMEMODE_H_
#define _GAMEMODE_H_

#include <stdio.h> 
#include <strings.h> 

void gamemodeManager();

#define ESCAPE_KEY 0x1B
#define CONTROL_KEY 200
#define ALT_KEY 202

#define IS_UPPER(c) ((c) >= 'A' && (c) <= 'Z') 
#define IS_LOWER(c) ((c) >= 'a' && (c) <= 'z')
#define IS_ALPHA(c) (IS_UPPER(c) || IS_LOWER(c))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

#define SECONDS 0
#define MINUTES 2
#define HOURS 4

#endif