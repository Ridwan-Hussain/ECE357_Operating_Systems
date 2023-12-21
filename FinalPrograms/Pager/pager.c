//Ridwan Hussain & Nicholas Storniolo - A simple version of
//more that echos the standard input to the standard output 
//for 23 lines and prints "---Press RETURN for more---", where 
//the user can enter a line for more input or hit "q"/"Q" to 
//quit the program.

#define BUFSIZE 4096 //This is overkill and should realistically be only the value of nc.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int pager(FILE* tty);

int main() {		
	FILE* tty;
	if (!(tty = fopen("/dev/tty", "r"))) {
		fprintf(stdout, "Error opening terminal for standard input with read permissions\nerrno: %d, strerrer: %s\n", errno, strerror(errno));
		return errno;
	}
	return pager(tty);
}

int pager(FILE* tty) {
	char termInput[BUFSIZE]; char inputStream[BUFSIZE]; char c;
	while ((c = getchar()) != EOF) {
		fprintf(stdout, "%c", c); //print out the char that was overwritten by getchar()
		for (int i = 0; i < 23; i++) {
			if ((fgets(inputStream, BUFSIZE, stdin)) == NULL) {
				if ((c = getchar()) == EOF) {
					return 0;
				}
			} else {
				fprintf(stdout, "%s", inputStream);
			}
		}
		fprintf(stdout, "---Press RETURN for more---");
		while ((c = fgetc(tty)) != '\n') { //Used as reference for fgetc: https://www.tutorialspoint.com/c_standard_library/c_function_fgetc.htm
			if (c == 'q' || c == 'Q') { 
				return 0;
			} else if (c == EOF) {
				fprintf(stdout, "\n");
				return 0;
			}
		}
	}
	return 0;
}

