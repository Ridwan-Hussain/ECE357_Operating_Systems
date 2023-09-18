// Ridwan Hussain - Program 1 meow
#define SIZEOFBUF 4096
#define FILENAMESIZE 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int oFlag(int argc, char** argv);
int openFile(char* filename, int flags);
int readFile(int fd, char (*buf)[SIZEOFBUF+1]);
//int writeToTerm(char (*buf)[SIZEOFBUF+1]);

int main(int argc, char **argv) {
	int fileIndex = 1, outFD = 1;
	/* Check if a user entered an output file */
	int inputComp = 0;
	int readN, readFD, writeN;
	int skipIndex = oFlag(argc, argv);
	char buffer[SIZEOFBUF+1];
	if (skipIndex) {
		outFD = open(argv[skipIndex+1], (O_RDWR|O_CREAT|O_TRUNC), 0666);
		if (outFD < 0) {
			printf("Error Occured with Output File Descriptor: errno - %d, strerror - %s\n", errno, strerror(errno));
			return errno;
		}
	}
	if (argc == 1 || (skipIndex && argc == 3)) {
		readFD = 0;
		while ( (readN = readFile(readFD, &buffer)) != 0) { /* Determine when at EOF */
			if (readN < 0) { /* Error catching */
				printf("Error Occured with Input File Descriptor: errno - %d, strerror - %s\n", errno, strerror(errno));
				break;
			} 
			if (readFD == 0 && buffer[readN-1] == '-1') {
				inputComp = 1;
			}
			writeN = write(outFD, buffer, readN);
		}
		
	}
	/* Read through each of the files */
	while (fileIndex < argc) {
		/*printf("argc: %d, argv: ", argc);
		for (int i = 0; i < argc; i++) {
			printf("%s, ", argv[i]);
		}
		printf("\n");*/
		/* Skip the file index for the "-o" and the following string */
		if (fileIndex == skipIndex) {
			fileIndex += 2;
			continue;
		/* stdin if the file name is "-" */
		} else if (!(strcmp(argv[fileIndex], "-"))) { 
			readFD = 0;
		} else {
			readFD = open(argv[fileIndex], O_RDONLY, 0666);
		}
		if (inputComp) {
			inputComp = 0;
		}
		while ( inputComp == 0 && (readN = readFile(readFD, &buffer)) != 0) { /* Determine when at EOF */
			if (readN < 0) { /* Error catching */
				printf("Error Occured with Input File Descriptor: errno - %d, strerror - %s\n", errno, strerror(errno));
				break;
			} 
			//printf("-->Buffer: %s-->buffer[readN]: %c", buffer, buffer[readN-1]);
			/*printf("readFD: %d, readN: %d\n", readFD, readN);
			for (int i = 0; i < readN; i++) {
				printf("i: %d --> buf: %c\n", i, buffer[i]);
			}*/
			if (readFD == 0 && buffer[readN-1] == '-1') {
				inputComp = 1;
				//break;
			}
			//char truncBuf[readN];
			//strncpy(truncBuf, buffer, readN);
			//printf("Writing to outFD\n");
			writeN = write(outFD, buffer, readN);
			//printf("Wrote to outFD\n");
			//printf("Wrote %d bytes\n", writeN);
		}
		//writeN = write(outFD, '\0', 1);
		//printf("Closing readFD\n");
		//printf("File Index: %d\n", fileIndex);
		if (readFD != 0) {
			close(readFD);
		}
		fileIndex++;
	}
	//printf("Closed readFD\n");
	if (skipIndex) {
		//printf("Closing outFD\n");
		close(outFD);
		//printf("Closed outFD\n");
	}
	return 0;
}

int oFlag(int argc, char** argv) {
	int opt;
	int pos = 0;
	while ( (opt=getopt(argc, argv, "o")) != -1) {
		pos++;
		switch (opt) {
			case 'o':
				return pos;
				break;
			default:
				break;
		}
	}
	return 0;
}

int openFile(char* filename, int flags) {
	return open(filename, flags, 0666);
}

int readFile(int fd, char (*buf)[SIZEOFBUF+1]){
	int readN = read(fd, buf, SIZEOFBUF);	 
	if (readN < SIZEOFBUF)
		for (int i = readN; i < SIZEOFBUF; i++) {(*buf)[i] = '\0'; }
	return readN;
}

/*int writeToTerm(char (*buf)[SIZEOFBUF+1]) {
	int writeN = write(1, buf, SIZEOFBUF);
	return writeN;
}*/


