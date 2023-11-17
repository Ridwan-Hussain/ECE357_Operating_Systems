//Ridwan Hussain & Nicholas Storniolo - The smear program is a find and replace program. It takes an input of some strlen as 
//the targets, takes a string of strlen as the replacement, and does this find & replace operation for any amount of target files.

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int smear(int fd, char* target, char* replacement, int fileSize);

int main(int argc, char* argv[]) {
	if (argc < 3) {
  		fprintf(stderr, "Not enough inputs for program. Syntax is`./smear TARGET REPLACEMENT file1 {file2 ...}`\nExiting program early.\n");
		return 0;
	} else if (strlen(argv[1]) != strlen(argv[2])) {
		fprintf(stderr, "Target and replacement strings need to be equal size. Exiting program early.\n");
		return 0;
	}
	int fileCount = argc - 3;
	char* target = argv[1];  char* replacement = argv[2];
	int fd; struct stat statBuf;
	for (int fileNum = 0; fileNum < fileCount; fileNum++) {
		char* file = argv[fileNum+3];
		if ((fd = open(file, O_RDWR)) < 1) {
			fprintf(stderr, "Error opening file %s. Skipping to next file.\nErrno: %d, Strerror: %s\n", file, errno, strerror(errno));
			continue;
		} else if (stat(file, &statBuf) == -1) {
			fprintf(stderr, "Could not stat the item %s. Skipping to next file.\n Errno: %d, Strerror: %s\n", file, errno, strerror(errno));
			close(fd);
			continue;
		}
		fprintf(stdout, "Current File: %s.\n", file);
		int replacements = smear(fd, target, replacement, statBuf.st_size);
		fprintf(stdout, "Number of replacements: %d.\n\n", replacements);
		close(fd);
	}
	return 0;
}

int smear(int fd, char* target, char* replacement, int fileSize) {
	char* fileContents = mmap(NULL, fileSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	int replacements = 0; int strLen = strlen(target);
	for (size_t pos = 0; pos < strlen(fileContents) - strLen; pos++) {
		//Incrementing string came from GeeksforGeeks: https://www.geeksforgeeks.org/get-a-substring-in-c/
		if (!(strncmp(fileContents+pos, target, strLen))) {
			replacements++;
			strncpy(fileContents+pos, replacement, strLen);
			pos += strLen-1;
		}
	}
	munmap(fileContents, fileSize);
	return replacements;
}
