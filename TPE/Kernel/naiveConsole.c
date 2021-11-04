#include <naiveConsole.h>

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

#define TOTAL_TEXT_SCREEN_LENGTH width*height
#define TOTAL_SCREEN_LENGTH width*height*2
#define DEFAULT_COLOR 0x0F

int current_print_color = DEFAULT_COLOR;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		if(string[i] == '\n')
			ncNewline();
		else
			ncPrintCharWithColor(string[i], current_print_color);
}

void ncPrintWithColor(const char * string, uint8_t color_code){
	int i;

	for (i = 0; string[i] != 0; i++){
		if(string[i] == '\n')
			ncNewline();
		else
			ncPrintCharWithColor(string[i], color_code);
	}
}

void ncPrintToPos(const char * string, int position){
	if(position < 0xB8000 || position >= 0xB8000 + TOTAL_SCREEN_LENGTH)
        return;

    char * aux = (char*) position;
    for(int i = 0; string[i] != 0; i++) {
		*(aux+2*i) = string[i];
	}
}

void ncPrintChar(char character)
{
	if(character == '\n')
		ncNewline();
	else {
	*currentVideo = character;
	currentVideo += 2;
	}
}

void ncPrintCharWithColor(char character, uint8_t color_code){
	if(character == '\n')
		ncNewline();
	else {
		*currentVideo = character;
		currentVideo++;
		*currentVideo = color_code;
		currentVideo++;
	}
}

void ncSetPrintColor(uint8_t color_code){
	current_print_color = color_code;
}

void ncClearPrintColor(){
	current_print_color = DEFAULT_COLOR;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
