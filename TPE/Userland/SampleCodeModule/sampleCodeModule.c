/* sampleCodeModule.c */
#include <stdio.h>
int scan(char *buffer, int size);

int main() {
	print("No puedo creer que esto funcione");

	// char c;
	// while ((c = getChar()) != '\n')
	// {	
	// 	if(c != 0)
	// 		putChar(c);
	// }
	
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