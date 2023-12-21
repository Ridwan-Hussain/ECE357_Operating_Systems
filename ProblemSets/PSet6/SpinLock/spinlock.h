//Ridwan Hussain & Nicholas Storniolo - Header filer for spinglock.c library.
//Contains all prototype declarations, struct definitions, defines, and includes.

#define UNLOCK 0
#define LOCK 1

#include "tas.h"
#include <sched.h>

struct spinlock {
	char lock;
	int pid;
	int numOfLocks;
	int numOfUnlocks;
};

void spin_lock(struct spinlock *l);
void spin_unlock(struct spinlock *l);
