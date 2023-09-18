// Program to test to see different system calls used to run programs from PSet1 Q1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
	//double e = exp(1.0);
	//int n = write(1,"XYZ",3);
	//int n =open("/ooopsy",O_WRONLY|O_CREAT|O_TRUNC,0666);
	//int fd = 10; char buf[3]; printf ("%d\n", read(fd,buf,3));
	//int fd =write(1,NULL,1);
	int fd = close(-1);
	
	printf("%d\n", fd);
	if (fd < 0){
		printf("Error Codes\n");
		printf("%d: %s\n", errno, strerror(errno));
	}
	return 0;
}
