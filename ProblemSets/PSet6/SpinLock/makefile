CFLAGS = -Wall -Wextra -Wpedantic

spinlock: spinLockTest.c spinlock.c spinlock.h tas.h tas64.S
	gcc -o spinLockTest spinLockTest.c tas64.o
	./spinLockTest 20

extra: spinLockTest.c spinlock.c spinlock.h tas.h tas64.S
	gcc -o spinlock spinLockTest.c $(CFLAGS)
