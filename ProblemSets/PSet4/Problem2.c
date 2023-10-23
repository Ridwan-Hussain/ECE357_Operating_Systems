#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int sigURGCounter;
int sigRTMINCounter;

void handler(int signalNum){
	if (signalNum == SIGURG) {
		sigURGCounter++;
		//fprintf(stdout, "SIGURG Received\n");
		fprintf(stdout, "SIGURGCounter = %d, SIGRTMINCounter = %d\n", sigURGCounter, sigRTMINCounter);
	} else if (signalNum == SIGRTMIN) {
		sigRTMINCounter++;
		//fprintf(stdout, "SIGRTMIN Received\n");
	}
}

int main() {
	signal(SIGURG, handler);
	signal(SIGRTMIN, handler);
	int victimPID;
	switch(victimPID = fork()) {
		case -1:
			fprintf(stderr, "Fork failed?\n");
			exit(-1);
		case 0: //Victim
			fprintf(stdout, "Victim Child going to sleep.\n");
			while(1);
			break;
		default:
			int sigGenPID, wStatus;
				switch(sigGenPID = fork()) {
					case -1:
						fprintf(stderr, "Fork failed??\n");
						exit(-1);
						break;
					case 0: //Bullies
						for (int i = 0; i < 100; i++) {
							if (kill(victimPID, SIGURG) == -1) {
								fprintf(stderr, "Error sending SIGURG\nErrno: %d, Strerror: %s\n", errno, strerror(errno));\
								exit(errno);
							}   
							if (kill(victimPID, SIGRTMIN) == -1) { 
								fprintf(stderr, "Error sending SIGRTMIN\nErrno: %d, Strerror: %s\n", errno, strerror(errno));
								exit(errno);
							} 
						}
						exit(0);
						break;
					default: // Main Parent
						while(wait(NULL) != -1) {
							//fprintf(stderr, "Signals sent.\n");
						}
						break;
				}
	}
	return 0;
}
