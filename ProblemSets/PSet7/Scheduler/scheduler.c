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
			} /*else {
				nice(0);
			} */

			while (1) {
				/*if (getrusage(RUSAGE_SELF, &rus[i]) == -1) {
					fprintf(stderr, "Error getting rusage for pid %d, i = %d. Exiting program.\n", getpid(), i);
					exit(0);
				}
				//Total time (us) = User time (seconds + useconds)                        + System time (seconds + useconds)
				//totalTimeRun = rus[i].ru_utime.tv_sec * 1000000 + rus[i].ru_utime.tv_usec + rus[i].ru_stime.tv_sec * 1000000 + rus[i].ru_stime.tv_usec;
				
				//fprintf(stderr, "i = %d, processWeight = %llu, multiplier = %ld, totalTimeRun = %llu\n"*/
			}			
			break;
		default:
			//fprintf(stdout, "Pre wait. Total wait time is approx %d.\n", waitPerProcess * numOfProcess);
			//sleep(waitPerProcess);
			/*if (i != 0) {
				exit(0);
			}*/
			break;
		}
	}
	fprintf(stderr, "Pre wait. Estimated wait time is at least %d.\n", waitPerProcess * numOfProcess);
	sleep(waitPerProcess * numOfProcess);

	//Kill and collect all children
	fprintf(stderr, "Post wait: COLLECTING ALL THE CHILDREN!\n");
	for (int i = 0; i < numOfProcess; i++) {
		if (kill(childPIDs[i], SIGKILL) == -1) {
			fprintf(stderr, "Failed to send SIGKILL to process %d, or i = %d. Exiting program.\n", childPIDs[i], i);
			exit(0);
		} else if (wait4(childPIDs[i], 0, 0, &rus[i]) == -1) { //Wait for each process and store the values
			fprintf(stderr, "Wait system call failed for process %d, i = %d. Exiting program.\n", childPIDs[i], i);
			exit(0);
		}
	}
	
	long totalTimeSec = 0; long totalTimeuSec = 0; double percentCPU;
	long processTotalTimeSec = rus[0].ru_utime.tv_sec + rus[0].ru_stime.tv_sec; 
	long processTotalTimeuSec = rus[0].ru_utime.tv_usec + rus[0].ru_stime.tv_usec; 
	for (int i = 0; i < numOfProcess; i++) {
		totalTimeSec += rus[i].ru_utime.tv_sec + rus[i].ru_stime.tv_sec; 
		totalTimeuSec += rus[i].ru_utime.tv_usec + rus[i].ru_stime.tv_usec; 
	}
	/*fprintf(stdout, "\nTotal Time         = %ld.%.6ld\nTotal Process Time = %ld.%6ld\n", totalTimeSec, totalTimeuSec, processTotalTimeSec, processTotalTimeuSec);
	fprintf(stdout, "\nUsertime:   %ld.%.6ld secs\nSystemtime: %ld.%.6ld secs\n", rus[0].ru_utime.tv_sec, rus[0].ru_utime.tv_usec, rus[0].ru_stime.tv_sec, rus[0].ru_stime.tv_usec);
	
	fprintf(stdout, "\n\nProcess 1 = %ld, Process 2 = %ld\n", rus[0].ru_utime.tv_sec, rus[1].ru_utime.tv_sec); 
	fprintf(stdout, "\nProcess 1 = %ld, Process 2 = %ld\n\n", rus[0].ru_utime.tv_usec, rus[1].ru_utime.tv_usec); */
	
	long totalTime = totalTimeSec * 1000000 + totalTimeuSec; long processTotalTime = processTotalTimeSec * 1000000 + processTotalTimeuSec;
	double CPUPercentage = (double) processTotalTime / totalTime * 100;
	//fprintf(stdout, "\nnumOfProcess, niceValue, waitPerProcess, totalTime, processTotalTime, CPUPercentage");
	fprintf(stdout, "| %d | %d | %d | %ld | %ld | %0.4f |\n",
		numOfProcess, niceValue, waitPerProcess, totalTime, processTotalTime, CPUPercentage);
	//fprintf(stdout, "\n");
	return 0;
}
