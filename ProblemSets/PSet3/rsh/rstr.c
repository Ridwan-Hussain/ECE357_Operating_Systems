//Ridwan Hussain - My own custom C-string helper library to help do
//common string commands such as copy, and append.
//Code used here was inspired by geeksforgeks.org/different-ways-to-copy-a-string-in-c-c/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Copys the input string and returns a copy of the string as a char pointer.
char* rcpStr(char* str) {
	char* str2, *p1, *p2;

	str2 = (char*)malloc(strlen(str)+1); 
	//plus 1 might not be necessary
	p1 = str;
	p2 = str2;

	while (*p1 != '\0') {
		*p2 = *p1;
		p1++;
		p2++;
	}
	*p2 = '\0';

	return str2;
}

//Appends str1 and str2 and returns an ouput strOut
//Should be able to repeat parameters and chain commands if desired.
char* rappendStr(char* str1, char* str2) {
	char *p1, *p2, *pOut;
	char* strOut = (char*)malloc(strlen(str1) + strlen(str2) + 1); 
	//^ might have too much space allocated here, need to double check.
	p1 = str1; p2 = str2; pOut = strOut;

	while (*p1 != '\0') {
		*pOut = *p1;
		p1++;
		pOut++;

	}
	while (*p2 != '\0') {
		*pOut = *p2;
		p2++;
		pOut++;
	}
	*pOut = '\0'; //Need to add NULL character at the end of the string.
	return strOut;

}

//Redefines strtok in a way I understand it.
char* rstrTok(char* str, char delim) {
	int numOfInputs = 0; int startWord = 0;
	char* *inputs;
	for (int i = 0; i < strlen(cmdline); i++) {
		if (cmdline[i] == ' ') {
			numOfInputs++;
			(*inputs) = malloc(i - startWord);
			for (int j = startWord; j < i - startWord; j++) {
				(*input) = cmdline;
				(*input)++;
				cmdline++;
			}
		}
	}
}

