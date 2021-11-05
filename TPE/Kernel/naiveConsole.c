#include <naiveConsole.h>

static char buffer[64] = {'0'};
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

#define TOTAL_TEXT_SCREEN_LENGTH width*height
#define TOTAL_SCREEN_LENGTH width*height*2
#define LINE_LENGTH width*2
#define COL_LENGTH height*2
#define LAST_LINE video + TOTAL_SCREEN_LENGTH - LINE_LENGTH
#define DEFAULT_COLOR 0x0F

// chars que ocupa el prompt
#define PROMPT_SIZE 5

void ncPrint(const char * string){
	ncPrintWithColor(string, DEFAULT_COLOR);
}

void ncPrintWithColor(const char * string, uint8_t color_code){
	if(currentVideo == LAST_LINE){
		ncScrollUp();
		currentVideo = LAST_LINE;
	}

	for (int i = 0; string[i] != 0; i++){
		if(string[i] == '\n')
			ncNewline();
		else
			ncPrintCharWithColor(string[i], color_code);
	}
}

void ncPrintInPos(const char * string, int row, int col, uint8_t colorCode){
	if(row < 0 || row > height)
		return;
	if(col < 0 || col > width)
		return;

	uint8_t * printPos = video + row * LINE_LENGTH + col * 2;

    for(int i = 0; string[i] != 0; i++) {
		// *(printPos+2*i) = string[i];
		// *(printPos+2*i+1) = colorCode;

		printPos[2*i] = string[i];
		printPos[2*i+1] = colorCode;
	}
}

void ncPrintChar(char character){
	ncPrintCharWithColor(character, DEFAULT_COLOR);
}

void ncPrintCharWithColor(char character, uint8_t color_code){
	if(currentVideo == LAST_LINE + LINE_LENGTH){
		ncScrollUp();
		currentVideo = LAST_LINE;
	}

	if(character == '\n')
		ncNewline();
	else {
		*currentVideo = character;
		currentVideo++;
		*currentVideo = color_code;
		currentVideo++;
	}
}

void ncNewline(){
	do {
		ncPrintChar(' ');
	} while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncScrollUp(){
	int i;
	for (i = 0; i < TOTAL_SCREEN_LENGTH - LINE_LENGTH; i++) {
		video[i] = video[i + LINE_LENGTH];
	}
	
	while(i < TOTAL_SCREEN_LENGTH){
		if(i % 2 == 0)
			video[i++] = ' ';
		else
			video[i++] = DEFAULT_COLOR;
	}
}

void ncBackspace(){
	//Si estoy al principio de la linea (adelante del prompt)
	uint64_t posInLine = (uint64_t)(currentVideo - video) % (uint64_t)(LINE_LENGTH);
	if(posInLine <= PROMPT_SIZE + 1)
		return;

	currentVideo--;
	*currentVideo = DEFAULT_COLOR;
	currentVideo--;
	*currentVideo = ' ';
}

void ncPrintDec(uint64_t value){
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value){
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear(){
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
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
