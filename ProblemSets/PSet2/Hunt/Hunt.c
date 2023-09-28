// Ridwan Hussain - Program 2 hunt
// Used pubs.opengroup.org opendir, readdir, stat, and other various examples
// as references while making the program.
#define MAXPATHNAMELENGTH 4096
#define SIZEOFBUF 5

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

/* Program Checklist
 * 1. Get File Name, store file bytes and file contents
 * 2. opendir, readdir, closedir, fprintf, strlen, chdir
 */

int fileSize;
int numOfBuffers;
//char fileContent;

int searchDir(char* pathName, char fileContent[numOfBuffers][SIZEOFBUF]);

int main(int argc, char *argv[]) {
	//Synthax is hunt <targetPath> <startPath>
	char *target = argv[1];
	char *startPath = argv[2];
	struct stat statBuf;
	stat(target, &statBuf);
	fileSize = statBuf.st_size;
	numOfBuffers = (fileSize/SIZEOFBUF) + 1;
	printf("numOfBuffers: %d\n", numOfBuffers);
	char fileContent[numOfBuffers][SIZEOFBUF];
	int targetFD = open(target, O_RDONLY, 0666);
	char buffer[SIZEOFBUF];
	for (int i = 0; i < numOfBuffers; i++) {
		read(targetFD, fileContent[i], SIZEOFBUF);
		printf("i: %d, fileContent: %s\n", i, fileContent[i]);
	}
	searchDir(startPath, fileContent);

	return 0;
}

int searchDir(char* pathName, char fileContent[numOfBuffers][SIZEOFBUF]) {
	// Code was referenced from opendir and readdir pages on pubs.opengroup.org
	DIR *currentDir;
	struct dirent *dirEntries;
	if ((currentDir = opendir(pathName)) == NULL) {
		printf("Current Dir - %s - is NULL. Print Error Message.\n", pathName);
		exit(1);
	}
	struct stat statBuf;
	char entryPath[MAXPATHNAMELENGTH];
	int status;
	while ((dirEntries = readdir(currentDir)) != NULL) {
		printf("Looking at file: %s\n", dirEntries->d_name);
		if (!(strcmp(dirEntries->d_name, ".")) || !(strcmp(dirEntries->d_name, ".."))) {
			continue;
		}
		//Might need to replace `dirEntries->d_name` with `entryPath`
		strcpy(entryPath, pathName);
		strcat(entryPath, dirEntries->d_name);
		printf("Entry Path: %s\n", entryPath);
		if (stat(entryPath, &statBuf) == -1) {
			printf("Could not find the item: \"%s, %s\".\n", entryPath, pathName);
			printf("Error #: %d, String Error: %s\n", errno, strerror(errno));
			//exit(2);
		} else if (S_ISDIR(statBuf.st_mode)) { //Check if entry is a directory
			strcat(entryPath, "/");
			if (statBuf.st_mode && (S_IRUSR || S_IXUSR)) {
				searchDir(entryPath, fileContent);
			} else {
				printf("Do not have proper permissions for opening file directory.\n");
				exit(3);
			}
		// if we have good permissions, assume person running the code is user
		//permissions = S_IRUSR, use && 
		} else if (!(statBuf.st_mode && S_IRUSR)) {
			printf("Do not have proper permissions\n");
			exit(4);
		} else if (S_ISREG(statBuf.st_mode)) {
			if (statBuf.st_size == fileSize) {
				printf("Same file Size!!!\n");
				int match = 1, readN;
				int matchFD = open(entryPath, O_RDONLY, 0666);
				char buffer[SIZEOFBUF];
				printf("Checkpoint 1!\n");
				for(int i = 0; i < numOfBuffers; i++) {
					printf("Checkpoint 2!\n");
					readN = read(matchFD, buffer, SIZEOFBUF);
					printf("Checkpoint 3!\n");
					if (strcmp(fileContent[i], buffer)) {
						printf("Checkpoint why?\n");
						printf("\ni: %d\nRead Buffer: %s\nFileContents: %s\n\n", i, buffer, fileContent[i]);	
						match = 0;
						break;
					} else {
						printf("Checkpoint 4!\n");
						printf("\ni: %d\nRead Buffer: %s\nFileContents: %s\n\n", i, buffer, fileContent[i]);
					}
					printf("Checkpoint 5!\n");
				}
				printf("Checkpoint 6!\n");
				if (match) {
					printf("SAME FILE CONTENTS TOOOOO!!!!!\n");
				}
			}
		} else if (S_ISLNK(statBuf.st_mode)) {
			printf("We've hit a symbolic LINK!!!!!!!!!\n");
		}
	}
	
	if (closedir(currentDir) == -1) {
		printf("PRINT ERROR MESSAGE!");
		printf("Error #: %d, String Error: %s", errno, strerror(errno));
	}
}

int readFile () {

}
