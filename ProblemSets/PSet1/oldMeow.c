// Ridwan Hussain - Program 1 meow
#define SIZEOFBUF 10
#define FILENAMESIZE 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/* GOALS:
 * Open a file
 * - add "./" at the beginning
 * Read a file
 * Write to a file
 * - If -o, output file
 *   - open w/ O_CREAT
 *   - overwrite old file?
 * - If no -o, stdout*/

int oFlag(int argc, char** argv);
int openFile(char* filename, int flags);
int readFile();
int writeFile();

int readFromTerm(int fd, char (*buf)[SIZEOFBUF+1]);
int writeToTerm(char (*buf)[SIZEOFBUF+1]);

int main(int argc, char **argv) {
	int startIndex = 1;
	char *outputFile;
	/* Check if a user entered an output file */
	if (int o_flag = oFlag(argc, argv)) {
		startIndex = 1;
		int out_fd = openFile(argv[2], O_CREATE);
	}
	
	int readn;
	/* Read through each of the files */
	while (startIndex < argc) {
		/* Make sure it's not at EOF */
		while ((readn = readFile() )!= 0) {
			/* Error catching */
			if (readn < 0) {
				continue;
			}
			/* Write to Buffer */
			if (o_flag) {
				/* Write to output file */
				
				}
			
			/* Write to terminal */
			writeToTerm(buffer);
			
			}
		startIndex++;
	}
	

	/*int o_flag = oFlag(argc, argv);
	printf("\no_flag: %d\n", o_flag);
	
	//int fd = open("./test.txt", 0);
	char buffer[10];
	read(0, buffer, SIZEOFBUF);
	
	int fd = openFile("test.txt", 0);
	printf("\nfd: %d\n", fd);
	//int readN = read(fd, buf, SIZEOFBUF);	
	//printf("%d", readN);
	int n, counter;
	char buf[SIZEOFBUF + 1];
	while (n = readFromTerm(fd, &buf) > 0) {
		//printf("counter: %d n: %d, ", ++counter, n);
		//printf("buf: %s\n", buf);
		writeToTerm(&buf);
		//printf(" wN: %d\n", wN);
		//Flushes out buf
	}
	close(fd);*/
	return 0;
}

int readFromTerm(int fd, char (*buf)[SIZEOFBUF+1]){
	int readN = read(fd, buf, SIZEOFBUF);	 
	if (readN < SIZEOFBUF)
		for (int i = readN; i < SIZEOFBUF; i++) {(*buf)[i] = (char) 0; }
	return readN;
}

int writeToTerm(char (*buf)[SIZEOFBUF+1]) {
	int writeN = write(1, buf, SIZEOFBUF);
	return writeN;
}

int openFile(char* filename, int flags) {
	//int fileSizeName = sizeof(filename);
	//char fN[67] = strcat("./", filename);
	/*char fN[fileSizeName + 2];
	fN[0] = '.';
	fN[1] = '/';
	for (int i = 2; i < fileSizeName + 2; i++) {
		fN[i] = filename[i-2];
	}*/
	/*char fN[fileSizeName];
	for (int i = 0; *(filename + i) != '\0'; i++) {
		fN[i] = filename[i];
	}*/
	printf("filename: %s|", filename);
	return open(filename, flags);
}

int oFlag(int argc, char** argv) {
	if (argc > 1) {
		return !(strcmp(argv[1], "-o"));
	}
	return 0;
	/*int o_flag = 0;
	if (strcmp(argv[1], "-o") == 0) {
		o_flag = 1;
	}
	return o_flag;*/
}
