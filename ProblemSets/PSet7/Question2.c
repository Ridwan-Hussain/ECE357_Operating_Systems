//Ridwan Hussain & Nicholas Storniolo - Simple read and write system call.

#include <stdio.h>
#include <unistd.h>

int main() {
	int bufSize = 4096;
	char buf[bufSize];
	int bytes = read(0, buf, bufSize);
	write(1, buf, bytes);
	_exit(0);
}

