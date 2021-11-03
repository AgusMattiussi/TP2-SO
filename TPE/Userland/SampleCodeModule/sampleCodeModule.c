/* sampleCodeModule.c */
#include <stdio.h>

int main() {
	print("No puedo creer que esto funcione");

	/*char c;
	while ((c = getChar()) != '\n')
	{	
	if(c != 0)
 		putChar(c);
	}*/

	char s[10];
	int len = scan(s, 10);
	print(s);

	return 0;
}
