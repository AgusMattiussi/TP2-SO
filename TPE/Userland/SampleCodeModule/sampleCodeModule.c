/* sampleCodeModule.c */
#include <strings.h>
#include <stdio.h>

int main() {
	
	char * str = "Hola mundo 123";
	char * tokens[3] = {0};
	int nextPos;

	strtok(str, ' ', tokens, 3);
	for (int i = 0; i < 3; i++){
		print(tokens[i]);
		print("//");
	}

	return 0;
}
