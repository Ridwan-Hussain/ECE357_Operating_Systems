//Ridwan Hussain & Nicholas Storniolo - Creating the task scheduler that runs a set of tasks based on nice values.

// Take in arguments from command line (Num of processes, nice value, wait time)
// call fork numOfProcess times, store pids in an array of stuff.
// Kill them after t time for each loop.
// Report the total time by children, and time nice program got.
// Need to implement formula somewhere with nice value.
//   - 10% difference in CPU allocation for each step. 
//   - If 2 processes, A = 0 & B=1, WA = 1.0 and WB = 0.8. Total Weight is 1.8
//   - wn = Wn/LW. With this example, A = 1.0/1.8 = 55.5%, B = 0.8/1.8 = 45.5%.
//   - Each step is a multiplier of 1.25.
//   - Check load value. If higher than X amount, sched_yield()

#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	int numOfProcess, niceValue, waitPerProcess;
	if (argc < 4) {
		fprintf(stderr, "Need to supply more arguments. Exiting prematurely.\n");
		return 0;
	} else if (!(numOfProcess = atoi(argv[1])) || numOfProcess < 2) {
		fprintf(stderr, "Number of processes (argv[1]) needs to be at least 2");
		return 0;
	} else if (!(waitPerProcess = atoi(argv[3])) || waitPerProcess < 0) {
		fprintf(stderr, "waitPerProcess (argv[3]) needs to be a number or greater than zero.\n");
		return 0;
	} else if (!(niceValue = atoi(argv[2]))) {
		niceValue = 0;
	} 
	
	fprintf(stdout, "Number of processes: %d, target process nice value: %d, wait per process: %d.\n");
	
	struct rusage *ru;
	int pid;

	for (int i = 0; i < numOfProcess) {
		switch (pid = fork()) {
		case -1: 
			fprintf(stderr, "Could not fork process at i = %d. Exiting early.\nErrno: %d, strerr: %s\n", i, errno, strerr(errno));
			return 0;
		case 0:  //Child
			
			break;
		default: //Parent
			sleep(waitPerProcess);
			break;
		}
	}

}



