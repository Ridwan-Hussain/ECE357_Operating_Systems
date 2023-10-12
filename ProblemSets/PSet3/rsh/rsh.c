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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <wstat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int shell(char* scripts);
int cmdLine(char* cmd, int prevStatus);
//int cat(char* parameters);
//int ls(char* parameters);
int cd(char* parameters);
int pwd(char* parameters);
int redirect(char* input);

int main(int argc, char* argv[]) {
	char *scripts;
	for (int i = 0; i < argc - 1; i++) {
		scripts = (char*)malloc(strlen(argv[i+1]));
		memcpy(scripts, argv[i+1], strlen(argv[i+1]));
	}
	
	//Print debugging
	fprintf(stdout, "Scripts = %s\n", scripts);
	fprintf(stdout, "Argv[1] = %s\n", argv[1]);
	
	return shell(scripts);
}

int shell(char* scripts) {
	int status = 0; //status will be called with exit with the most recent errno from a command or my own defined value.
					//-1 is defined as invalid exit status, -2 is defined as command not found.
	if (sizeof(scripts)) {
		//Referred to https://solarianprogrammer.com/2019/04/03/c-programming-read-file-lines-fgets-getline-implement-portable-getline/ 
		//for reading files line by line in C
		FILE* scriptFP; char* cmdline; size_t lineLen = 0; 
		if ((scriptFP = fopen(scripts, "r")) == NULL) {
			fprintf(stdout, "Error Opening Script File %s\nError Number: %d, String Error: %s", scripts, errno, strerror(errno));
			free(scripts);
			return errno;
		}
		while(getline(&cmdline, &lineLen, scriptFP) != -1) {
			status = cmdLine(cmdline, status);
			fprintf(stdout, "Read a line from input: %s\n", cmdline);
		}
		fprintf(stdout, "Script finished running.\n");
		free(scripts);
		return errno;
	}
	char userInput[BUFSIZE];
	while(1) { //forever loop
		fprintf(stdout, "While loop runnin.\n");
		//Reference GeeksForGeeks for how to use fgets: https://www.geeksforgeeks.org/taking-string-input-space-c-3-different-methods/
		fgets(userInput, BUFSIZE, stdin);
		status = cmdLine(userInput, status);
	}
	return status; //this returns after all the scripts have been read if there were scripts
}

int cmdLine(char* cmdline, int prevStatus) { 
	//Referenced GeeksForGeeks on how to use strtok: https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
	char delim[] = " ";
	char* cmd = strtok(cmdline, delim);
	char* input = strtok(0, delim);
	return 0;	
	/*if (!(strcmp(cmd, "cat"))) {
		fprintf(stdout, "Read cat.\n");
		return cat(input);
	} else if (!(strcmp(cmd, "ls"))) {
		fprintf(stdout, "Read ls.\n");
		return ls(input);
	} else */if (!(strcmp(cmd, "cd"))) {
		fprintf(stdout, "Read cd.\n");
		return cd(input);
	} else if (!(strcmp(cmd, "pwd"))) {
		fprintf(stdout, "Read pwd.\n");
		return pwd(input);
	} else if (!(strcmp(cmd, "#"))) { //char cmp instead of strcmp
		fprintf(stdout, "Read #.\n");
		return prevStatus;
	} else if (!(strcmp(cmd, "exit"))) {
		fprintf(stdout, "Read exit.\n");
		if (status = atoi(input)) {
			fprintf(stdout, "Exited program successfully with new value %d\n", status);
			exit(status);
		} else if (input == NULL) {
			fprintf(stdout, "Exited program successfully with previous value %d\n", prevStatus);
			exit(prevStatus);
		} else {
			//Since errno is normally 0 or + value, I set status = -1 if there's an error with exiting.
			fprintf(stderr, "Error exiting program. Exit value was not an integer. Exiting with value -1.\n");
			exit(-1);
		}
	} else {
		fprintf(stdout, "Read cmd %s.\n", cmd);
		return binCmd(cmd, cmdline)
	}
}

int binCmd(char* cmd, char* cmdline) { 
	int pid;
	switch (pid = fork()) {
		case -1:
			//Give more info on this
			fprintf(stderr, "Fork failed in binCmd.\n");
			break;
		case 0:
			/*child logic stuff*/
			fprintf(stdout, "Inside child program.\n");
			if (execvp(cmd, cmdline) == -1) {
				fprintf(stderr, "Execvp Failed.\n");
				return -1;
			}
			break;
		default: 
			/*Parent logic wait stuff*/
			fprintf(stdout, "Inside parent program.\n");
			struct rusage ru;
			unsigned wstatus;
			if (wait4(pid, &wstatus, 0, &ru) == -1) {
				fprintf(stderr, "Wait system call failed\n");
				return errno;
			}
			fprintf(stdout, "Parent is done waiting.\n");
			fprintf(stdout, "Child process %d, consumed time %ld.%.6d seconds of usertime \n", pid, ru.ru_utime.tv_sec, ru.ru_utime.tv_usec);
			if (wstatus != 0) {
				if (WIFSIGNALED(wstatus)) {
					fprintf(stderr, "Exited with signal %d\n", WTERMSIG(wstatus));
					return WTERMSIG(wstatus);
				} else {
					fprintf(stderr, "Exited with non-zero return value %d\n", WEXITSTATUS(wstatus));
					return WEXITSTATUS(wstatus);
				}
			}
			return wstatus; //should be 0 at this point
	}
}

int cd(char* parameters, char* delim) { 
	int pid;
	while (paramters != NULL) {
		redirect(parameter);
		switch (pid = fork()) {
		case -1:
			//Give more info on this
			fprintf(stderr, "Fork failed in cd.\n");
			break;			
		case 0:
			fprintf(stdout, "Inside child for cd.\n");
			if (chdir(parameter) == -1) {
				fprintf(stderr, "Error with chdir for path %s: \nErrno: %d, Strerr: %s\n", parameters, errno, strerrno(errno));

			} 
		default:
			fprintf(stdout, "Inside parent for cd.\n");
			

		}
		parameters = strtok(0, delim);
	}
}


int pwd(char* parameters) { 
	
}


int redirect(char* input) {
	//(input)    - no redirection, 
	//(<input)   - openfile & redirect stdin
	//(>input)   - open/create/truncate file and redirect stdout
	//(2>input)  - open/create/truncate file and redirect stderr
	//(>>input)  - open/create/append file and redirect stdout
	//(2>>input) - open/create/append file and redirect stderr
	//dup3 for actual redirection with file descriptor table
	if (strncmp("<", input, 1)) {
		input += 1;
		int originalFD = open(input, O_RDONLY);	
		dup3(originalFD, 0, O_CLOEXEC);
	} else if (strncmp(">", input, 1)) {
		input += 1;
		int originalFD = open(input, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		dup3(originalFD, 1, O_CLOEXEC);
	} else if (strncmp("2>", input, 2)) {
		input += 2;
		int originalFD = open(input, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		dup3(originalFD, 2, O_CLOEXEC);
	} else if (strncmp(">>", input, 2)) {
		input += 2;
		int originalFD = open(input, O_WRONLY|O_CREAT|O_APPEND, 0666);
		dup3(originalFD, 1, O_CLOEXEC);
	} else if (strncmp("2>>", input, 3)) {
		input += 3;
		int originalFD = open(input, O_WRONLY|O_CREAT|O_APPEND, 0666);
		dup3(originalFD, 2, O_CLOEXEC);
	} else {
		int originalFD = open(input, O_RDONLY);
	}
	return originalFD;
}
