#include <strings.h>

int strcmp(const char * s1, const char * s2){

    while (*s1 != 0){
        if(*s1 != *s2)
            return 1;
        s1++;
        s2++;
    }

    return !(*s2 == 0);
}

unsigned int strlen(const char * str){
    int count = 0;
    
    while (*str != 0){
        count++;
        str++;    
    }
    return count;
}

int strtok(char * input, char delim, char * tokenArray[], int maxTokens){
	int tokCount = 0;
	if (*input != delim && *input != 0)
		tokenArray[tokCount++] = input;

	while (*input != 0){
		if (*input == delim){
			*input = 0;
            char * next = input + 1;
			if (*next != delim && *next != 0){
				if (tokCount >= maxTokens)
					return tokCount;
				tokenArray[tokCount++] = next;
			}
		}
		input++;
	}
	return tokCount;
}

void itoa(int num, char *str){
    int digits = 1;
    for(int n = num / 10 ; n != 0 ; digits++, n/=10);

    if(digits == 1){
        str[0] = '0';
        str[1] = num + '0';
        str[2] = 0;
        return;
    }

    str[digits] = 0;
    for(int i=digits-1; i >= 0; i--){
        str[i] = (num % 10) + '0';
        num /= 10;
    }
    return;
}

static int pow(int base, unsigned int exp){
	int rta = 1;
	for (int i = 0; i < exp; i++)
		rta *= base;
	return rta;
}

unsigned long hexaStringToInt(char *s){
    int c;
	unsigned long rta = 0;

	if (s[0] == '0' && s[1] == 'x')
		s += 2;

	int len = strlen(s);

	for (int i = 0; i < len; i++){

		c = s[len-1-i] - '0';

		if (c < 0 || c > 9){
			c = s[len - 1 - i] - 'A' + 10;
			if (c < 10 || c > 15)
				return 0;
		}

		rta += c * pow(16, i);
	}
	return rta;
}

char convertCharToUpperCase(char c) {
    return (c >= 'a' && c <= 'z') ? c + 'A' - 'a' : c;
}