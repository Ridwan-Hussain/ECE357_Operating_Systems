#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main() {
	int pid, fd; char buf [4096];
	switch(pid=fork()) {
		case -1: 
			perror("fork"); 
			return -1;
		case 0:
			fd = open("x.out", O_RDWR);
			read(fd, buf, 10);
			break;
		default:
			fd = open("x.out", O_WRONLY|O_APPEND); 
			dup2(fd, 1); 
			break;
	}
	write(fd, "ABC", 3);
	write(1, "WXYZ", 4);

	return 0;
}
