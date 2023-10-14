#include <string.h>
#include <stdio.h>

#define SIZEOFBUF 4096

void funFun(int* val) {
	*val = 5;
}

int funFunct(char* input, char* delim, int counter) {
	printf("\nThis is from the function: %s\n", input);
	while(input != 0) {
		printf("Index#: %d, Input: %s\n", ++counter, input);
		input = strtok(0, delim);
	}
	printf("Function end.\n\n");
}

int main() {
	/*char input[SIZEOFBUF];
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

	printf("\n\n\n");*/
	/*char test[] = "This is a test input.x";
	printf("test strlen: %d\ntest output: %d\n", strlen(test), strcspn(test, "x"));*/

	int randInt = 5;
	for (int i = 0; i < 5 && 1; i++) {
		printf("i: %d \n", i);
	}
	int values[2] = {0, 3};
	fprintf(stdout, "value: %d\n", values[0]);
	funFun(&values[0]);
	fprintf(stdout, "value: %d\n", values[0]);
	return 0;
}
