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

int launcher(char wordgenArg[]);
int waiting(char* program, int pid);

int main(int argc, char* argv[]) {
	if (argc > 1) {
		return launcher (argv[1]);
	}
	return launcher("0");
}

int launcher(char wordgenArg[]) {
	//The program will fork three times. Each of the forks occur in the parent.
	//The first child is wordgen. The 2nd child wordsearch. The third child is pager.
	//The final parent is for the rest of the logic for the code.
	int wordgenPID=0, wordsearchPID=0, pagerPID=0;
	int pipe1[2], pipe2[2];
	if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
		fprintf(stderr, "Failed to make pipes.\n Errno: %d, strerror: %s.\n", errno, strerror(errno));
		exit(errno);
	} 
	//Each child will close dup as needed to stdin/stdout then close all the pipes.
	switch (wordgenPID=fork()) {
		case -1:
			fprintf(stderr, "Fork failed for wordgen.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //wordgen, write pipe1[1]
			fprintf(stdout, "In wordgen child.\n");
			dup2(pipe1[1], 1);
			close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
			if (execlp("./wordgen", "./wordgen", wordgenArg, NULL) == -1) {
				fprintf(stderr, "Error with exec-ing wordgen.\nErrno: %d, strerror: %s\n", errno, strerror(errno));
				exit(errno);
			}
			break;
		default:
			fprintf(stdout, "First parent.\n");
			waiting("wordgen", wordgenPID);
			
	switch (wordsearchPID = fork()) {
		case -1:
			fprintf(stderr, "Fork failed for wordsearch.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //wordsearch, read pipe1[0], write pipe2[1];
			fprintf(stdout, "In wordsearch child.\n");
			dup2(pipe1[0], 0); dup2(pipe2[1], 1);
			close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
			if (execlp("./wordsearch", "./wordsearch", "dict.txt", NULL) == -1) {
				fprintf(stderr, "Error with exec-ing wordsearch.\nErrno: %d, strerror: %s\n", errno, strerror(errno));
				exit(errno);
			}
			break;
		default: 
			fprintf(stdout, "Second parent.\n");
			waiting("wordsearch", wordsearchPID);
	
	switch (pagerPID = fork()) {
		case -1:
			fprintf(stderr, "Fork failed for pager.\nErrno: %s, strerror: %s\n", errno, strerror(errno));
			return(errno);
		case 0: //pager, read pipe2[0]
			fprintf(stdout, "In pager child.\n");
			dup2(pipe2[0], 0);
			close(pipe1[0]); close(pipe1[1]); close(pipe2[0]); close(pipe2[1]);
			if (execlp("./pager", "./pager", NULL) == -1) {
				fprintf(stderr, "Error with exec-ing pager.\nErrno: %d, strerror: %s\n", errno, strerror(errno));
				exit(errno);
			}
			break;
		default: //main program logic
			fprintf(stdout, "Final parent.\n");
			waiting("pager", pagerPID);		
			break;
		}}} //spaced out so code isn't a rat's nest.
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
