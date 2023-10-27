//Ridwan Hussain & Nicholas Storniolo - A simple version of
//more that echos the standard input to the standard output 
//for 23 lines and prints "---Press RETURN for more---", where 
//the user can enter a line for more input or hit "q"/"Q" to 
//quit the program.

#define BUFSIZE 4096 //This is overkill and should realistically
					 //be only the value of nc.

#include <stdio.h>

int pager();

int main() {		
	pager();
	return 0;
}

int pager() {
	char userInput[BUFSIZE]; char c;
	while ((c = getchar()) != EOF && c == '\n') {
		for (int i = 0; i < 23; i++) {
			if ((fgets(userInput, BUFSIZE, stdin)) == NULL) {
				if ((c = getchar()) == EOF) {
					fprintf(stdout, "Read intermediate EOF. Terminating.\n");
					return 0;
				}
			} else {
				fprintf(stdout, "%s", userInput);
			}
		}
		fprintf(stdout, "---Press RETURN for more---\n");
	}
	return 0;
}

