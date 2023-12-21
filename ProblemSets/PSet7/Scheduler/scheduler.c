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
	

// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!
// NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!! NEED TO ADD ERRNO AND STRERROR FOR ERROR CHECKING!!!

#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include "spinlock.c"

int loadWeight;

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
	
	struct rusage ru;
	struct spinlock* sLock = mmap(NULL, sizeof(struct spinlock), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	int childPIDs[numOfProcess];
	struct rusage rus[numProcess];
	int pid; int nice; int processWeight;
	int lowestWeight;
	
	
	for (int i = 0; i < numOfProcess; i++) {
		switch (pid = fork()) {
		case -1: 
			fprintf(stderr, "Could not fork process at i = %d. Exiting early.\nErrno: %d, strerr: %s\n", i, errno, strerr(errno));
			return 0;
		case 0:  //Child
			if (i == 0) {
				nice = niceValue; //Child 0 is the target process whose nice value we change
			} else {
				nice = 0;
			} 


			while (1) {
				//Weight is from the 1.25 to the nice value divided by the load (total) weight.
				if (getrusage(RUSAGE_SELF, rus[i]) == -1) {
					fprintf(stderr, "Error getting rusage for pid %d, i = %d. Exiting program.\n", getpid(), i);
					exit(0);
				}

				spin_lock(sLock);
				loadWeight += 
				spin_unlock(sLock);
				processWeight = pow(1.25, nice) / loadWeight;
				
				if (processWeight > lowestWeight) {
					sched_yield();
				}
			}
			
			break;
		default: //Parent
			childPIDs[i] = pid;
			sleep(waitPerProcess);
			break;
		}
	}
	
	//Collect all children
	kill(childPIDs[0], SIGKILL);
	if (wait4(childPIDs[0], 0, 0, rus[0]) == -1) {
		fprintf(stderr, "Wait system call failed for target process, pid = %d. Exiting program.\n", childPIDs[0]);		
		exit(0);
	}
	
	for (int i = 1; i < numberOfProcess; i++) {
		if (kill(childPIDs[i], SIGKILL) == -1) {
			fprintf(stderr, "Failed to send SIGKILL to process %d, or i = %d. Exiting program.\n", childPIDs[i], i);
			exit(0);
		} else if (wait(0) == -1) { //Wait for each process and store the values
			fprintf(stderr, "Wait system call failed for process %d, i = %d. Exiting program.\n", childPIDs[i], i);
			exit(0);
		}
	}
	fprintf(stderr, "Usertime: %ld.%.6d secs\nSystemtime: %ld.%.6d secs\n", 
		rus[0].ru_utime.tv_sec, rus[0].ru_utime.tv_usec, rus[0].ru_stime.tv_sec, rus[0].ru_stime.tv_usec);

	return 0;
}



