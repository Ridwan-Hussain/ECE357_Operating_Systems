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
#include <EVERYTHINGELSE.H>

int cmdLine(char* cmd, int prevStatus);
int cat(char* parameters);
int ls(char* parameters);
int cd(char* parameters);
int pwd(char* parameteres);

int main(int argc, char* argv[]) {
	char* scripts;
	if (argc > 1) {
		scripts = &argvc[1];
	}
	return shell(scripts);
}

int shell(char** scripts) {
	if (sizeof(scripts) == 0) {
		size_t numOfScripts = 0;
	} else {
		size_t numOfScripts = sizeof(scripts) / sizeof(scripts[0]);
	}
	FILE scriptFP;
	int status = 0; //status will be called with exit with the most recent errno from a command or my own defined value.
					//-1 is defined as invalid exit status, -2 is defined as command not found
	for (int i = 0; i < numOfScripts; i++) {
		if (scriptFP = fopen(scripts[i], "r")) == NULL) {
			printf(stderr, "Error Opening Script File %s\nError Number: %d, String Error: %s", scripts[i], errno, strerror(errno));
			continue;
		}
		for (int j = 0; j < scripts[i]Lines; j++) {
			status = cmdLine(scripts[i][j]);
		}
	}
	char* userInput;
	while(!(numOfScripts)) { //forever loop
		scanf("%s", &userInput);
		status = cmdLine(userInput, status);
	}
	return status; //this returns after all the scripts have been read if there were scripts
}

int cmdLine(char* cmd, int prevStatus) { 
	//Tokenize inputs first (maybe). Use strtok or something
	//This is also where all the commands get their redirection stuff
	
	//For simplicity sake, going to allow arg1 < arg2 as a redirect but not arg1<arg2
	
	if (!(strcmp(cmd, "cat") || strcmp(cmd, "meow"))) {
		status = cat();
	} else if (!(strcmp(cmd, "ls"))) {
		status = ls();
	} else if (!(strcmp(cmd, "cd"))) {
		status = cd();
	} else if (!(strcmp(cmd, "pwd"))) {
		status = pwd();
	} else if (!(strcmp(cmd, "#"))) { //char cmp instead of strcmp
		return prevStatus;
	} else if (!(strcmp(cmd, "exit") {
		if (cmd[1] == NULL) {
			exit(prevStatus);
		} else if (cmd[1] != int) {
			//Since errno is normall 0 or + value, I set status = -1 if there's an error with exiting.
			printf(stderr, "Error exiting program. Exit value was not an integer. Exiting with value -1.\n");
			exit(-1);
		} else {
			exit(cmd[1]);
		}
	} else {
		printf(stderr, "Not a supported command for this shell script.\n");
		return -2;
	}
}

int cat(char* parameters) { }
int ls(char* parameters) { }
int cd(char* parameters) { }
int pwd(char* parameteres) { }



