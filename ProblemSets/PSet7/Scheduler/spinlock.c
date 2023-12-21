//Library with spinlock implementation.

#include "spinlock.h"

void spin_unlock(struct spinlock *l) {
	(l->lock) = 0;
	return;
}

void spin_lock(struct spinlock *l) {
	while (tas(&l->lock) == LOCK) {
		sched_yield();
	}
	return;
}
