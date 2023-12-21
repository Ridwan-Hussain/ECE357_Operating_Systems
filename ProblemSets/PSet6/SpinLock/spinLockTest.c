//Ridwan Hussain & Nicholas Storniolo - Program that tests the spinlock.c library.
//Does a non-atomic operation (addition) to confirm that without the spinlock program output incorrect value.

#include "spinlock.c"
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	//fprintf(stderr, "\nargv[0]=|%s|\nargv[1]=|%s|\n", argv[0], argv[1]);
	int numOfChildren;
	if (argc < 2) {
		fprintf(stderr, "Need more inputs.\n");
		return 0;
	} else if (!(numOfChildren = atoi(argv[argc-1]))) {
		fprintf(stderr, "Final argument is not an integer. Exiting program prematurely.\n\n");
		return 0;
	}
	int numOfTrials = 200000; int constant = 5;
	unsigned long long* pcounter = mmap(NULL, sizeof(long long), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	struct spinlock* sLock = mmap(NULL, sizeof(long long), PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED, -1, 0);
	*pcounter = 0; //Initialize value to 0
		
	int spawnerPid, pid;
	/*switch (spawnerPid = fork()) {
	case -1: 
		fprintf(stderr, "Initial fork failed.\n");
		return 0;
	case 0: //Parent
		break;
	default:
		for (int i = 0; i < numOfTrials/2; i++) {
			for (int j = 0; j < numOfChildren; j++) {
				switch (pid = fork()) {
				case -1:
					fprintf(stderr, "Fork failed at i = %d, j = %d.\n", i, j);
					break;
				case 0: //Parent
					//sLock->pid = pid; 
					spin_lock(sLock); //(sLock->numOfLocks)++;
					(*pcounter)++;
					spin_unlock(sLock); //(sLock->numOfUnlocks)++;	
					break;
				default: //Child
					//sLock->pid = pid; 
					spin_lock(sLock); //(sLock->numOfLocks)++;
					(*pcounter)++;
					spin_unlock(sLock); //(sLock->numOfUnlocks)++;
					break;
				} 
			}
		}
	}
	if (spawnerPid || pid) { //This is the parent that spawned all the children.
		return 0;
	}*/
	
	for (int i = 0; i < numOfTrials; i++) {
		for (int j = 0; j < numOfChildren; j++) {
			switch (pid = fork()) {
			case -1:
				fprintf(stderr, "Fork failed at i = %d, j = %d.\n", i, j);
				break;
			case 0: //Parent
				break;
			default: //Child
				sLock->pid = pid; 
				(sLock->numOfLocks)++;
				(sLock->numOfUnlocks)++;
				spin_lock(sLock); //(sLock->numOfLocks)++;
				*pcounter = *pcounter + constant; //Doing +2 instead of +1 so operation requires more lines of assembly, therefore more likely to break.
				spin_unlock(sLock); //(sLock->numOfUnlocks)++;
				return 0;
			} 
		}
	}

	for (int i = 0; i < numOfTrials*numOfChildren; i++) {
		wait4(-1, NULL, 0, NULL);
	}
	*pcounter = *pcounter / constant;	
	fprintf(stdout, "Initialize Counter value to 0.\n");
		
	unsigned long long expectedOutput = numOfTrials * numOfChildren;
	//Do funny math here to calculate expected output
	fprintf(stdout, "Expected Output: %llu.\n", expectedOutput);

	fprintf(stdout, "Actual Output: %llu.\n", *pcounter);
	
	fprintf(stdout, "Was off by %llu.\n", expectedOutput-*pcounter);
	return 0;
}

