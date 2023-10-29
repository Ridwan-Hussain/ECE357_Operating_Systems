//Ridwan Hussain & Nicholas Storniolo - Launcher program that runs the command
//./wordgen ARG1 | ./wordsearch dict.txt | ./pager where ARG1 is an optional 
//argument of launcher. 

#define BUFSIZE 4096

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int launcher(int wordgenArg);
int waiting(char* program, int pid);

int main(int argc, char* argv[]) {
	int wordgenArg = 0;
	if (argc > 1) {
		wordgenArg = atoi(argv[1]);
	}
	return launcher(wordgenArg);
}

int launcher(int wordgenArg) {
	//The program will fork three times. Each of the forks occur in the parent.
	//The first child is wordgen. The 2nd child wordsearch. The third child is pager.
	//The final parent is for the rest of the logic for the code.
	int wordgenPID=0, wordsearchPID=0, pagerPID=0;;
	switch (wordgenPID=fork()) {
		case -1:
			fprintf(stderr, "Fork failed for wordgen.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //wordgen
			fprintf(stdout, "In wordgen child.\n");
			execlp("./wordgen", "./wordgen", wordgenArg, NULL);
			break;
		default:
			fprintf(stdout, "First parent.\n");
			waiting("wordgen", wordgenPID);
			
	switch (wordsearchPID = fork()) {
		case -1:
			fprintf(stderr, "Fork failed for wordsearch.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //wordsearch
			fprintf(stdout, "In wordsearch child.\n");
			execlp("./wordsearch", "./wordsearch", "dict.txt", NULL);
			break;
		default: 
			fprintf(stdout, "Second parent.\n");
			waiting("wordsearch", wordsearchPID);
	
	switch (pagerPID = fork()) {
		case -1:
			fprintf(stderr, "Fork failed for pager.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //pager
			fprintf(stdout, "In pager child.\n");
			execlp("./pager", "./pager", NULL);
			break;
		default: //main program logic
			fprintf(stdout, "Final parent.\n");
			waiting("pager", pagerPID);
			
			break;
		}}} //spaced out so code isn't nested like crazy.
	return 0;	
}

int waiting(char* program, int pid) {
	unsigned wstatus;
	wait4(pid, &wstatus, 0, NULL);
	fprintf(stdout, "Finished waiting for %s child.\n", program);
	if (wstatus != 0) {
		if (WIFSIGNALED(wstatus)) {
			fprintf(stderr, "%s exited with signal: %d\n", program, WTERMSIG(wstatus));
			exit(WTERMSIG(wstatus));
		} else {
			fprintf(stderr, "%s exited with return value: %d\n", program, WEXITSTATUS(wstatus));
			exit(WEXITSTATUS(wstatus));
		}
	}
}
