#ifndef _STRINGS_H
#define _STRINGS_H

int strcmp(const char * s1, const char * s2);
unsigned int strlen(const char * str);
int strtok(char * input, char delim, char * tokenArray[], int maxTokens);
void itoa(int num, char *str);

#endif