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
void fileValiditor(FILE* dictFP, int lines);

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
		maxlencounter++;
		if (c == '\n') {
			lines++;
			if (maxlencounter > maxlen) {
				maxlen = maxlencounter;
			}
			maxlencounter = 0;
		} else if (c <= 64 || (c >= 91 && c <= 96) || c >= 123) {
			maxlencounter = 0;
			lines++;
			while(c != '\n') { c = getc(dictFP); }
		}
	}
	rewind(dictFP);

	//Store dictionary into string array

	char **dict = (char **)malloc((lines)*sizeof(char**));
	for (int i  = 0; i < lines; i++) {
		dict[i] = (char *)malloc(maxlen*sizeof(char*));
	}

	char* dictLine; size_t lineLen = 0; int skip = 0; 
	for (int i = 0; getline(&dictLine, &lineLen, dictFP) != EOF; i++) {
		skip = 0;
		for (int j = 0; j < strlen(dictLine); j++ ) {
			if (dictLine[j] >= 65 && dictLine[j] <= 90) { //if character is upper case it's fine
				continue;
			} else if (dictLine[j] >= 97 && dictLine[j] <= 122) { //if character is lowercase convert it to uppercase
				dictLine[j] -= 32;
			} else if (dictLine[j] == '\n') { //If newline, replace with null
				dictLine[j] = '\0';
			} else { //otherwise skip it
				skip = 1;
				break;
			}
		}

		if (!(skip)) {
			strncpy(dict[i], dictLine, strlen(dictLine));
		}
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
		
		/*if (userInput == NULL || strlen(userInput) < 3 || strlen(userInput) > nc) { 
			continue;
		}*/
		
		userInput[strlen(userInput) - 1] = '\0'; //replaces \n with \0
		for (int i = 0; i < lines; i++) {
			if (!strcmp(userInput, dict[i])) {
				fprintf(stdout, "Found a match: %s\n", dict[i]); //can have duplicate matches
				matches++;
			}
		}
	}
}


/*void fileValiditor(FILE* dictFP, int lines) {
	
}*/
