#include <string.h>
#include <stdio.h>

#define SIZEOFBUF 4096

int funFunct(char* input, char* delim, int counter) {
	printf("\nThis is from the function: %s\n", input);
	while(input != 0) {
		printf("Index#: %d, Input: %s\n", ++counter, input);
		input = strtok(0, delim);
	}
	printf("Function end.\n\n");
}

int main() {
	char input[SIZEOFBUF];
	fgets(input, SIZEOFBUF, stdin);
	printf("Input: %s\n", input);
	char delim[] = " ";
	char* firstInput = strtok(input, delim);
	int counter = 0;
	printf("Index#: %d, Input: %s\n", ++counter, firstInput);
	char* tok = strtok(0, delim);
	funFunct(tok, delim, counter);
	while(tok != 0) {
		printf("Index#: %d, Input: %s\n", ++counter, tok);
		tok = strtok(0, delim);
	}
	printf("firstInput: %s, tok: %s\n", firstInput, tok);
	return 0;
}
