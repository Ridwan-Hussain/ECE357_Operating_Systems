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
int writeCtrl(int argc, char **argv, int skipIndex, int outFD);
int writeBuf(int outFD, int readFD);
int readFile(int fd, char (*buf)[SIZEOFBUF+1]);

/*int main(int argc, char **argv) {
	int outFD = 1;
	int skipIndex = oFlag(argc, argv); 
	if (skipIndex) {
		outFD = open(argv[skipIndex+1], (O_RDWR|O_CREAT|O_TRUNC), 0666);
		if (outFD < 0) {
			fprintf(stderr, "Error Occured with Output File Descriptor\nerrno - %d, strerror - %s\n", errno, strerror(errno));
			return -1;
		}
	}
	return writeCtrl(argc, argv, skipIndex, outFD);
}*/

int oFlag(int argc, char** argv) {
	/* Determines if the -o flag is present and the position */
	int opt, position = 0;
	while ( (opt=getopt(argc, argv, "o")) != -1) {
		position++;
		switch (opt) {
			case 'o':
				return position;
				break;
			default:
				break;
		}
	}
	return 0;
}

int writeCtrl(int argc, char **argv, int skipIndex, int outFD) {
	/* Main Control logic on what to write, when to write, and where to write */
	int fileIndex = 1, readFD;
	/* Special case if no input files are given for meow */
	if (argc == 1 || (skipIndex && argc == 3)) {
		readFD = 0;
		if (writeBuf(outFD, readFD) == -1) {
			return -1;
		}
	}
	/* Read through each of the files */
	while (fileIndex < argc) {
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
		if (writeBuf(outFD, readFD) == -1) {
			return -1;
		}
		fileIndex++;
	}
	if (skipIndex) {
		close(outFD);
	}
	return 0;
}

int writeBuf(int outFD, int readFD) {
	/* Correctly reads and writes the buffer to correct output and closes the file. */
	int readN;
	char buffer[SIZEOFBUF+1];
	while ( (readN = readFile(readFD, &buffer)) != 0) { /* Determine when at EOF */
		if (readN < 0) { /* Error catching */
			fprintf(stderr, "Error Occured with Input File Descriptor\nerrno - %d, strerror - %s\n", errno, strerror(errno));
			return -1;
		} 
		write(outFD, buffer, readN);
	}
	if (readFD != 0) {
		close(readFD);
	}
}

int readFile(int fd, char (*buf)[SIZEOFBUF+1]){
	/* Reads the files and replaces the rest of the buffer with NULL if there's nothing left to be read.*/
	int readN = read(fd, buf, SIZEOFBUF);	 
	if (readN < SIZEOFBUF)
		for (int i = readN; i < SIZEOFBUF; i++) {(*buf)[i] = '\0'; }
	return readN;
}
