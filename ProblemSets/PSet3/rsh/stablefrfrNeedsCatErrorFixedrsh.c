//Ridwan Hussain - Program 3 RSH (simple shell)

/* Program Requirements:
 * - Inside the while loop
 * - Read the stdin or an inputFile and parse it
 *   - Anything that starts with a # is a comment, no command (this is for script file)
 * - Tokenize input
 * - Fork for the new process if needed for a command; commands to fork include:
 *   - 
 * - Child process does all the I/O redirect stuff
 *   - Failure of a redirect should exit with 1
 *   - Do exec for the command
 *     - ls doesn't need full pathname to run
 * - Parent process does a specific wait system call
 *   - return the value of the child exist status or signal that killed a process
 *     - DON'T EXIT PROGRAM IF CMD FAILS, just report them (cmd, item affects, 
 *   - Also return the real time CPU duration for the command to execute b/w fork and wait being completed
 *     - can be found using wait3 --> times --> getrusage systemcall
 *   - Exit status if MSB 8 bits of WEXITSTATUS if voluntarily kill, LSB 8 bit of WEXITSTATUS if involuntarily
 * - Print output prompts
 * - 
 */

//Commands cat, ls, cd, pwd

/* CURRENT KNOWN BUGS:
 * - EOF 
 * - Bin command with single input argument
 */

#define BUFSIZE 4096
int numOfArgs;
int originalFD;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>

int shell(char* scripts);
int cmdLine(char* cmd, int prevStatus);
int binCmd(char** args); 
int cd(char** args); 
int pwd();
int redirect(char* *input, int* originalFD);

int main(int argc, char* argv[]) {
	char *scripts;
	for (int i = 0; i < argc - 1; i++) {
		scripts = (char*)malloc(strlen(argv[i+1]));
		memcpy(scripts, argv[i+1], strlen(argv[i+1]));
	}
	struct cmdstuff {
		char *scripts;
		int status;
		char *delim;
		char *cmd;
		char *cmdline;
		char *parameters;
		char *input; 
	} cmdDummy;
	//Print debugging
	fprintf(stdout, "Scripts = %s\n", scripts);
	
	return shell(scripts);
}

int shell(char* scripts) {
	int status = 0; //status will be called with exit with the most recent errno from a command or my own defined value.
					//-1 is defined as invalid exit status, -2 is defined as command not found.
	if (scripts != NULL) {
		//Referred to https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/ 
		//for reading files line by line in C
		FILE* scriptFP; char* cmdline; size_t lineLen = 0; 
		if ((scriptFP = fopen(scripts, "r")) == NULL) {
			fprintf(stdout, "Error Opening Script File %s\nError Number: %d, String Error: %s\n", scripts, errno, strerror(errno));
			free(scripts);
			return errno;
		}
		while(getline(&cmdline, &lineLen, scriptFP) != -1) {
			status = cmdLine(cmdline, status);
			//fprintf(stdout, "Read a line from input: %s\n", cmdline);
		}
		fprintf(stdout, "Script finished running.\n");
		free(scripts);
		return errno;
	}
	char userInput[BUFSIZE]; int c;
	while(11) { //forever loop
		fprintf(stdout, "While loop runnin.\n");
		//strcpy(userInput, "\0");
		while ((fgets(userInput, BUFSIZE, stdin)) == NULL) {
			while ((c = getchar()) != '\n' && c != EOF);
		}//Referenced GeeksForGeeks for how to use fgets: https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/
		if (userInput == NULL || strlen(userInput) == 1) {
			status = 0;
			fprintf(stdout, "Achieved if statement, returning.\n");
			return 0; //continue;
		}
		fprintf(stderr, "Last \\n: %d, last char %c, strlen: %d\n", strcspn(userInput, "\n"), userInput[strlen(userInput)-1], strlen(userInput));
		//userInput[strcspn(userInput, "\n")] = '\0'; //https://www.geeksforgeeks.org/removing-trailing-newline-character-from-fgets-input/
		//userInput[strcspn(userInput, EOF)] = '\0';
		userInput[strlen(userInput) - 1] = '\0';
		//if (strcspn(userInput, EOF) == strlen(userInput) - 1)
		numOfArgs = 1;
		for (int i = 0; i < strlen(userInput); i ++ ) {
			if (userInput[i] == ' ') { numOfArgs++; }
		}
		/*if (numOfArgs == 1) {
			numOfArgs++;
		}*/
		fprintf(stderr, "NumOfArgs: %d\n", numOfArgs);
		fprintf(stderr, "UserInput: |%s|\n", userInput);
		status = cmdLine(userInput, status);
		//while ((c = getchar()) != '\n' && c != EOF && c != '\0') { }
	}
	return status; //this returns after all the scripts have been read if there were scripts
}

int cmdLine(char* cmdline, int prevStatus) { 
	//Referenced GeeksForGeeks on how to use strtok: https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
	char delim[] = " ";	char *(args[numOfArgs+1]); 
	args[0] = strtok(cmdline, delim); args[numOfArgs] = NULL;
	int fd[numOfArgs]; fd[0] = 0;
	//char* cmd = strtok(cmdline, delim);
	//char* input = strtok(0, delim);
	int pid;
	fprintf(stdout, "numOfArgs: %d\nargs[0] = |%s|\n", numOfArgs, args[0]);
	for (int i = 1; i < numOfArgs && args[i-1] != NULL; i++) {
		args[i] = strtok(0, delim); 
		//input = strtok(0, delim);
		//PRINT DEBUGGING GOING ON HERE
		fprintf(stderr, "Before redirect args[%d] = |%s|\n", i, args[i]);
		redirect(&(args[i]), &fd[i]);
		fprintf(stderr, "After redirect args[%d] = |%s|\n", i, args[i]);
	}
	//start real time stuff here
	if (!(strcmp(args[0], "cd"))) {
		fprintf(stdout, "\nRead cd.\n");
		return cd(args);
	} else if (!(strcmp(args[0], "pwd"))) {
		fprintf(stdout, "\nRead pwd.\n");
		return pwd();
	} else if (!(strncmp(args[0], "#", 1))) {
		fprintf(stdout, "\nRead #.\n");
		return prevStatus;
	} else if (!(strcmp(args[0], "exit"))) {
		fprintf(stdout, "\nRead exit.\n");
		if (prevStatus = atoi(args[1])) { //can cause SEG FAULTS
			fprintf(stderr, "Exited program successfully with new value %d\n", prevStatus);
			exit(prevStatus);
		} else if (args[1] == NULL) {
			fprintf(stderr, "Exited program successfully with previous value %d\n", prevStatus);
			exit(prevStatus);
		} else {
			//Since errno is normally 0 or + value, I set status = -1 if there's an error with exiting.
			fprintf(stderr, "Error exiting program. Exit value was not an integer. Exiting with value -1.\n");
			exit(-1);
		}
	} else {
		switch (pid = fork()) {
			case -1: 
				//Give more info on this
				fprintf(stderr, "Fork failed in binCmd.\n");
				break;
			case 0:
				fprintf(stderr, "\nRead cmd: %s.\n", args[0]);
				return binCmd(args);
			default:
				/*Parent logic wait stuff*/
				//gettimeofday or clock
				fprintf(stderr, "Inside parent program for binCmd.\n");
				struct rusage ru;
				unsigned wstatus;
				if (wait4(pid, &wstatus, 0, &ru) == -1) {
					fprintf(stderr, "Wait system call failed\n");
					return errno;
				}
				fprintf(stderr, "Parent is done waiting.\n");
				fprintf(stderr, "Child process %d, consumed time %ld.%.6d seconds of usertime \n", pid, ru.ru_utime.tv_sec, ru.ru_utime.tv_usec);
				if (wstatus != 0) {
					if (WIFSIGNALED(wstatus)) {
						fprintf(stderr, "Exited with signal: %d\n", WTERMSIG(wstatus));
						return WTERMSIG(wstatus);
					} else {
						fprintf(stderr, "Exited with non-zero return value: %d\n", WEXITSTATUS(wstatus));
						return WEXITSTATUS(wstatus);
					}
				}
				fprintf(stderr, "Exited with ZERO return value :D\n");
				for (int i = 0; i < numOfArgs; i++) {
					if (fd[i]) {
						close(fd[i]);
					}
				}
				//close(originalFD);
				return wstatus; //should be 0 at this point
		}
	}
}

int binCmd(char** args) { 
	fprintf(stderr, "Inside child program for binCmd\n");
	if (execvp(args[0], args) == -1) {
		fprintf(stderr, "Execvp Failed for CMD: %s\nErrno: %d, Strerror: %s\n", args[0], errno, strerror(errno));
		return errno;
	}
	fprintf(stderr, "Command ran successfully.\n");
	return 0;
}

int cd(char** args) { 
	int pid;
	fprintf(stdout, "Inside child for cd.\n");
	fprintf(stderr, "Parameters: %s\n", args[1]);
	if (args[1] == NULL) {
		if (chdir(getenv("HOME")) == -1) {
			fprintf(stderr, "Error with chdir for HOME\nErrno: %d, Strerr: %s\n", errno, strerror(errno));
			return errno;
		}
	} else if (chdir(args[1]) == -1) {
		fprintf(stderr, "Error with chdir for path: %s\nErrno: %d, Strerr: %s\n", args[1], errno, strerror(errno));
		return errno;
	} 
	return 0;
}

int pwd() { 
	char cwd[BUFSIZE];
	fprintf(stdout, "Inside child for pwd.\n");
	if (getcwd(cwd, BUFSIZE) == NULL) {
		fprintf(stderr, "Error with getcwd.\nErrno: %d, Strerr: %s\n", errno, strerror(errno));
		return errno;
	}
	fprintf(stderr, "Path: %s\n", cwd);
	return 0;
}

int redirect(char** input, int* originalFD) {
	//(input)    0 - no redirection 
	//(<input)   1 - openfile & redirect stdin
	//(>input)   2 - open/create/truncate file and redirect stdout
	//(2>input)  3 - open/create/truncate file and redirect stderr
	//(>>input)  4 - open/create/append file and redirect stdout
	//(2>>input) 5 - open/create/append file and redirect stderr
	//dup3 for actual redirection with file descriptor table
	if (!(strncmp("<", *input, 1))) {
		*input += 1;
		fprintf(stderr, "Input: |%s|\n", *input);
		*originalFD = open(*input, O_RDONLY);	
		dup2(*originalFD, 0);
		//close(*originalFD);
		return 1;
	} else if (!(strncmp(">", *input, 1))) {
		*input += 1;
		fprintf(stderr, "Input: |%s|\n", *input);
		*originalFD = open(*input, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		dup2(*originalFD, 1);
		//close(*originalFD);
		return 2;
	} else if (!(strncmp("2>", *input, 2))) {
		*input += 2;
		fprintf(stderr, "Input: |%s|\n", *input);
		*originalFD = open(*input, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		dup2(*originalFD, 2);
		//close(*originalFD);
		return 3;
	} else if (!(strncmp(">>", *input, 2))) {
		*input += 2;
		fprintf(stderr, "Input: |%s|\n", *input);
		*originalFD = open(*input, O_WRONLY|O_CREAT|O_APPEND, 0666);
		dup2(*originalFD, 1);
		//close(*originalFD);
		return 4;
	} else if (!(strncmp("2>>", *input, 3))) {
		*input += 3;
		fprintf(stderr, "Input: |%s|\n", *input);
		*originalFD = open(*input, O_WRONLY|O_CREAT|O_APPEND, 0666);
		dup2(*originalFD, 2);
		//close(*originalFD);
		return 5;
	}
	fprintf(stderr, "Input: |%s|\n", *input);
	*originalFD = open(*input, O_RDONLY);
	return 0;
}
