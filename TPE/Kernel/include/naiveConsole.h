#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncPrintWithColor(const char * string, uint8_t color_code);
void ncPrintCharWithColor(char character, uint8_t color_code);
void ncChangeBackgroundColor(uint8_t color_code);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void ncPrintInPos(const char * string, int row, int col, uint8_t colorCode);
int getCurrentPos();
void ncScrollUp();
void ncBackspace();

// COLORS
#define BLUE_BLACK 0x01
#define GREEN_BLACK 0x02
#define CYAN_BLACK 0x03
#define RED_BLACK 0x04
#define MAGENTA_BLACK 0x05
#define ORANGE_BLACK 0x06
#define GREY_BLACK 0x07
#define YELLOW_BLACK 0x0E
#define WHITE_BLACK 0x0F

#endif