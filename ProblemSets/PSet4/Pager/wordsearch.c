//Ridwan Hussain & Nicholas Storniolo - From a given file input, take in a list of words
//as potential matches, and check if those words appear in stdin. If a match occurs, 
//echo the word to standard output.

#define BUFSIZE 4096

#include <stdio.h>
#include <errno.h>
#include <string.h>

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
	fclose(dictFP);
	return 0;
}

void wordSearch(FILE* dictFP) {
	char* dictLine; size_t lineLen = 0;
	char c; int lines = 0;
	
	//Count Number of lines
	for (c = getc(dictFP); c != EOF; c = getc(dictFP)) {
		if (c == '\n') {
			lines++;
		}
	}
	rewind(dictFP);
	
	//Store dictionary into string array
	char dict[lines][BUFSIZE]; //+1 here is for the \0 
	for (int i = 0; getline(&dictLine, &lineLen, dictFP) != EOF; i++) {
		strncpy(dict[i], dictLine, strlen(dictLine)-1);
		dict[i][strlen(dictLine)-1] = '\0';
	}

	char userInput[BUFSIZE]; int matches = 0;
	while(1) {
		while((fgets(userInput, BUFSIZE, stdin) == NULL)) {
			if ((c = getchar()) == EOF) {
				fprintf(stdout, "Total Matches: %d\n", matches);
				return;
			}
		}

		if (userInput == NULL) { //maybe OR strlen(userInput) < 3)
			continue;
		}
		userInput[strlen(userInput) - 1] = '\0';
		for (int i = 0; i < lines; i++) {
			if (!strcmp(userInput, dict[i])) {
				fprintf(stdout, "Found a match: %s\n", dict[i]);
				matches++;
			}
		}
		
	}

}
