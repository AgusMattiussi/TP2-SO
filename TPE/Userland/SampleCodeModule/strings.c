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

// int strtok(char * input, char delim, char * tokenArray[], int maxTokenCount){

//     int tokCount = 0;
//     int currentChar = 0;
//     while(*input != 0 && tokCount < maxTokenCount && currentChar < 20){
//         if(*input != delim){
//             tokenArray[currentChar++][tokCount] = *input;
//         } else { 
//             tokenArray[tokCount][currentChar] = 0;
//             tokCount++;
//             currentChar = 0;
//         }
//     }
//     tokenArray[tokCount][currentChar] = 0;
//     return tokCount;
// }

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


// char* strtok(char* s, char d)
// {
//     // Stores the state of string
//     static char* input = NULL;
  
//     // Initialize the input string
//     if (s != NULL)
//         input = s;
  
//     // Case for final token
//     if (input == NULL)
//         return NULL;
  
//     // Stores the extracted string
//     char* result = new char[strlen(input) + 1];
//     int i = 0;
  
//     // Start extracting string and
//     // store it in array
//     for (; input[i] != '\0'; i++) {
  
//         // If delimeter is not reached
//         // then add the current character
//         // to result[i]
//         if (input[i] != d)
//             result[i] = input[i];
  
//         // Else store the string formed
//         else {
//             result[i] = '\0';
//             input = input + i + 1;
//             return result;
//         }
//     }
  
//     // Case when loop ends
//     result[i] = '\0';
//     input = NULL;
  
//     // Return the resultant pointer
//     // to the string
//     return result;
// }