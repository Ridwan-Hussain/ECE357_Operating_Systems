//Ridwan Hussain & Nicholas Storniolo - Launcher program that runs the command
//./wordgen ARG1 | ./wordsearch dict.txt | ./pager where ARG1 is an optional 
//argument of launcher. 

#define BUFSIZE 4096

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int launcher(int wordgenArg);

int main(int argc, char* argv[]) {
	int wordgenArg = 0;
	if (argc > 1) {
		wordgenArg = atoi(argv[1]);
	}
	return launcher(wordgenArg);
}

int launcher(int wordgenArg) {
	//The program will fork twice. Once normally, and the parent will fork a second time.
	//The first child is wordgen. The 2nd child (so the child in the parent) is wordsearch.
	//The final parent is pager. 
	int wordgenPID=0, wordsearchPID=0;
	switch (wordgenPID=fork()) {
		case -1:
			fprintf(stderr, "Fork failed for wordgen.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //wordgen
			fprintf(stdout, "In wordgen child.\n");
			execvp("./wordgen", "./wordgen", wordgenArg, NULL);
			break;
		default:
			unsigned wordgenStatus;
			fprintf(stdout, "First parent.\n");
			wait4(wordgenPID, wordgenStatus, 0, NULL);
			fprintf(stdout, "Finished waiting for wordgen child.\n");
			
			if (wordgenStatus != 0) {
				if (WIFSIGNALED(wordgenStatus)) {
					fprintf(stderr, "Wordgen exited with signal: %d\n", WTERMSIG(wordgenStatus));
					return WTERMSIG(wordgenStatus);
				} else {
					fprintf(stderr, "Wordgen exited with return value: %d\n", WEXITSTATUS(wordgenStatus));
					return WEXITSTATUS(wordgenStatus);
				}
			}
			
			switch (wordsearchPID = fork()) {
				case -1:
				fprintf(stderr, "Fork failed for wordsearch.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
				return(errno);
			case 0: //wordsearch
				fprintf(stdout, "In wordsearch child.\n");
				execvp("./wordsearch", "./wordsearch", "dict.txt", NULL);
				break;
			default: //pager
				unsigned wordsearchStatus;
				fprintf(stdout, "Second parent.\n");
				wait4(wordsearchPID, wordsearchStatus, 0, NULL);
				fprintf(stdout, "Finished waiting for wordsearch child.\n");
			
				if (wordsearchStatus != 0) {
					if (WIFSIGNALED(wordsearchStatus)) {
						fprintf(stderr, "Wordsearch exited with signal: %d\n", WTERMSIG(wordsearchStatus));
						return WTERMSIG(wordsearchStatus);
					} else {
						fprintf(stderr, "Wordsearch exited with return value: %d\n", WEXITSTATUS(wordsearchStatus));
						return WEXITSTATUS(wordsearchStatus);
					}
				} 
				
				execvp("./pager", "./pager", NULL);
				break;
			}
	}
	
}

