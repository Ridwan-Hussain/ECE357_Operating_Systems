//Ridwan Hussain & Nicholas Storniolo - Creating the task scheduler that runs a set of tasks based on nice values.
	
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int numOfProcess, niceValue, waitPerProcess;
	if (argc < 4) {
		fprintf(stderr, "Need to supply more arguments. Exiting prematurely.\n");
		return 0;
	} else if (!(numOfProcess = atoi(argv[1])) || numOfProcess < 2) {
		fprintf(stderr, "Number of processes (argv[1]) needs to be at least 2\n");
		return 0;
	} else if (!(waitPerProcess = atoi(argv[3])) || waitPerProcess < 0) {
		fprintf(stderr, "waitPerProcess (argv[3]) needs to be a number or greater than zero.\n");
		return 0;
	} else if (!(niceValue = atoi(argv[2]))) {
		niceValue = 0;
	} 
	if (niceValue < 0 || niceValue > 20) {
		fprintf(stderr, "Nice Value out of bounds, setting nice value to 0.\n");
		niceValue = 0;
	}
	fprintf(stderr, "Number of processes: %d, target process nice value: %d, wait per process: %d.\n", numOfProcess, niceValue, waitPerProcess);
	
	int childPIDs[numOfProcess]; struct rusage rus[numOfProcess];	
	for (int i = 0; i < numOfProcess; i++) {
		switch (childPIDs[i] = fork()) {
		case -1: 
			fprintf(stderr, "Could not fork process at i = %d. Exiting early.\nErrno: %d, strerr: %s\n", i, errno, strerror(errno));
			return 0;
		case 0:  //Child
			if (i == 0) {
				nice(niceValue); //Child 0 is the target process whose nice value we change
			}                    //every other process gets the default nice value of 0.
			while (1); //infinite loop until child receives SIGKILL from parent
			break; //unneeded
		default:
			break;
		}
	}
	fprintf(stderr, "Pre wait. Estimated wait time is at least %d.\n", waitPerProcess * numOfProcess);
	//Total wait time is usually greater than estimated wait time because other processes are running.
	sleep(waitPerProcess * numOfProcess);

	//Kill and collect all children
	fprintf(stderr, "Post wait: COLLECTING ALL THE CHILDREN!\n");
	for (int i = 0; i < numOfProcess; i++) {
		if (kill(childPIDs[i], SIGKILL) == -1) {
			fprintf(stderr, "Failed to send SIGKILL to process %d, or i = %d. Errno: %d, strerr: %s.\nExiting program.\n", childPIDs[i], i, errno, strerror(errno));
			exit(0);
		} else if (wait4(childPIDs[i], 0, 0, &rus[i]) == -1) { //Wait for each process and store the values
			fprintf(stderr, "Wait system call failed for process %d, i = %d. Errno: %d, strerr: %s.\nExiting program.\n", childPIDs[i], i, errno, strerror(errno));
			exit(0);
		}
	}
	
	// User time (secs) * 10e6 + User time (usecs) + System time (secs) * 10e6 + System time (usecs)
	long processTotalTime = rus[0].ru_utime.tv_sec * 1000000 + rus[0].ru_utime.tv_usec + rus[0].ru_stime.tv_sec * 1000000 + rus[0].ru_stime.tv_usec; 
	long totalTime = processTotalTime;
	for (int i = 1; i < numOfProcess; i++) {
		totalTime += rus[i].ru_utime.tv_sec * 1000000 + rus[i].ru_utime.tv_usec + rus[i].ru_stime.tv_sec * 1000000 + rus[i].ru_stime.tv_usec; 
	}
	double CPUPercentage = (double) processTotalTime / totalTime * 100;
	
	fprintf(stdout, "| %d | %d | %d | %ld | %ld | %0.4f |\n",
		numOfProcess, niceValue, waitPerProcess, totalTime, processTotalTime, CPUPercentage);
	
	return 0;
}
