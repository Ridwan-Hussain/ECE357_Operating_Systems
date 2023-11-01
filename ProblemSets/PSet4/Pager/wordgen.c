//Ridwan Hussain & Nicholas Storniolo - Generate a random list of numOfWords between 3 and nc characters.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int wordgen(int numOfWords, int nc);

int main(int argc, char* argv[]) {
	int nc = 8; //MAX NUMBER OF CHARACTERS FOR A WORD
	if (argc > 1) {
		return wordgen(atoi(argv[1]), nc);
	}
	return wordgen(0, nc);
}

int wordgen(int numOfWords, int nc) {
	int infFlag = !(numOfWords); //if numOfWords = 0, run loop infinitely
	int charASCII, wordLen;
	char word[nc+1];
	srand(time(NULL)); //Random seed generated from current time for "dynamic" randomness, from Prof. Hak
	while (infFlag || numOfWords) {
		//rand() function was referenced from https://www.javatpoint.com/random-function-in-c
		wordLen = (unsigned) (rand() % nc) + 1; //referenced from https://c-faq.com/lib/randrange.html
		if (wordLen < 3) { //Minium wordLength is 3
			wordLen = 3; 
		}
		for (int i = 0; i < wordLen; i++) {
			charASCII = (unsigned) (rand() % 26); 
			word[i] = charASCII + 65; //+65 because that's where 'A' starts
		}
		word[wordLen] = '\0';
		fprintf(stdout, "%s\n", word);
		numOfWords--;
	}
	return 0;
}
