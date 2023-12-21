//Ridwan Hussain & Nicholas Storniolo - From a given file input, take in a list of words
//as potential matches, and check if those words appear in stdin. If a match occurs, 
//echo the word to standard output.

#define BUFSIZE 4096

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <setjmp.h>

int matches;
jmp_buf jmpbuf;

void handler(int signalNumber);
void wordSearch(FILE* dictFP);

int main(int argc, char* argv[]) {
	FILE* dictFP;
	if (argc < 2) {
		fprintf(stderr, "Error, no file input was given. Exiting program.\n");
		return 1;
	} else if ((dictFP = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "Error Opening Dictionary File %s\nError Number: %d, StrError: %s\n", argv[1], errno, strerror(errno));
		return errno;
	}
	wordSearch(dictFP);
	fprintf(stdout, "Total Matches: %d\n", matches); //Left for stdout if redirecting output
	fprintf(stderr, "Total Matches: %d\n", matches); 
	fclose(dictFP);
	return 0;
}

void handler(int signalNumber) {
	if (signalNumber == SIGPIPE) {
		longjmp(jmpbuf, 1);
	}
}

void wordSearch(FILE* dictFP) {
	if (setjmp(jmpbuf)) {
		return; 
	}
	FILE* goodDictFP; 
	if ((goodDictFP = fopen("goodDict.txt", "w+")) == NULL) {
		fprintf(stderr, "Error opening new file goodDict.txt.\nErrno: %d, Strerror: %s\n", errno, strerror(errno));
	}
	char c; int lines = 0; int nc = 8; //This nc variable is copied from wordgen.c
	int maxlen = 0; int maxlencounter = 0;
	signal(SIGPIPE, handler);
	
	//Count Number of lines
	for (c = getc(dictFP); c != EOF; c = getc(dictFP)) {
		if (c == '\n') {
			lines++;
			}
		}
	}
	rewind(dictFP);

	//Store dictionary into string array
	dict [lines][BUFSIZE]; 
	char* dictLine; size_t lineLen = 0; int skip = 0; 
	for (int i = 0; getline(&dictLine, &lineLen, dictFP) != EOF; i++) {
		strncpy(dict[i], dictLine, strlen(dictLine)-1);
		dict[i][strlen(dictLine)-1] = '\0'; 
	}


	char userInput[BUFSIZE];
	while(1) {
		while((fgets(userInput, BUFSIZE, stdin) == NULL)) {
			if ((c = getchar()) == EOF) {
				return;
			}
		}
		
		if (userInput == NULL || strlen(userInput) < 3) { 
			continue;
		}
		
		userInput[strlen(userInput) - 1] = '\0'; //replaces \n with \0
		for (int i = 0; i < lines; i++) {
			if (!strcmp(userInput, dict[i])) {
				fprintf(stdout, "Found a match: %s\n", dict[i]); //can have duplicate matches
				matches++;
			}
		}
	}
}
