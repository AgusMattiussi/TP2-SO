/* sampleCodeModule.c */
#include <stdio.h>
int scan(char *buffer, int size);

int main() {
	print("No puedo creer que esto funcione    ");

	putChar('H');
	putChar('O');
	putChar('L');
	putChar('A');
	putChar(' ');
	putCharWithColor('M', RED_BLACK);
	putCharWithColor('U', ORANGE_BLACK);
	putCharWithColor('N', YELLOW_BLACK);
	putCharWithColor('D', GREEN_BLACK);
	putCharWithColor('O', BLUE_BLACK);

	return 0;
}

int scan(char *buffer, int size)
{
	char c = 0;
	int len = 0;
	while ((c = getChar()) != '\n' && len < size)
	{
		putChar(c);
		buffer[len] = c;
		len++;
	}
	buffer[len] = '\0';
	return len;
}