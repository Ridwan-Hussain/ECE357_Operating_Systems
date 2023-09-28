// Ridwan Hussain - Program 2 hunt
// Used pubs.opengroup.org opendir, readdir, stat, and other various examples
// as references while making the program.
// Jacob Koziej (EE'25) also gave me advice on using mmap 
// Also referenced https://www.youtube.com/watch?v=m7E9piHcfr4
#define MAXPATHNAMELENGTH 4096
#define SIZEOFBUF 4096

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

/* Program Checklist
 * 1. Get File Name, store file bytes and file contents
 * 2. opendir, readdir, closedir, fprintf, strlen, chdir
 * 3. Skip dirs that don't have r-x permissions. If original path name 
 * is not viewable, report critical error; target file must be readable 
 * as well.
 * 4. Seg faults when symlinked to a file in a directory with not proper perms
 * */

int fileSize;
int numOfBuffers;
int targetFD;

int searchDir(char* pathName);

int main(int argc, char *argv[]) {
	//Synthax is hunt <targetPath> <startPath>
	char *target = argv[1];
	char *startPath = argv[2];
	struct stat statBuf;
	stat(target, &statBuf);
	fileSize = statBuf.st_size;
	int targetNLinks;
	
	targetFD = open(target, O_RDONLY, 0666);
	searchDir(startPath);

	return 0;
}

int searchDir(char* pathName) {

	// Code was referenced from opendir and readdir pages on pubs.opengroup.org
	DIR *currentDir;
	struct dirent *dirEntries;
	if ((currentDir = opendir(pathName)) == NULL) {
		printf("Current Dir - %s - is NULL. Print Error Message.\n", pathName);
		exit(1);
	}
	struct stat statBuf, statLinkBuf;
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
			if (lstat(entryPath, &statLinkBuf) == 0 && S_ISLNK(statLinkBuf.st_mode)) {
				printf("We've hit a symbolic LINK!!!!!!!!!\n");
				if (statBuf.st_size == fileSize) {
					printf("Same file Size!!!\n");
					int entryFD = open(entryPath, O_RDONLY, 0666);
					char *fileTarget = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, targetFD, 0);
					char *fileEntry = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, entryFD, 0);
					//printf("fileTarget:\n-----------------------------------------------------\n%s", fileTarget);
					//printf("fileEntry:\n-----------------------------------------------------\n%s", fileEntry);
					if (!(strcmp(fileTarget, fileEntry))) {
						printf("SAME FILE CONTENTS WOOOOOOOOOOOO\n");
					}
					close(entryFD);
				}
				//int readLinkN = readlink(entryPath, 
			} else {
				printf("Could not find the item: \"%s, %s\".\n", entryPath, pathName);
				printf("Error #: %d, String Error: %s\n", errno, strerror(errno));
				//exit(2);
			}
		} else if (S_ISDIR(statBuf.st_mode)) { //Check if entry is a directory
			strcat(entryPath, "/");
			if (statBuf.st_mode && (S_IRUSR || S_IXUSR)) {
				searchDir(entryPath);
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
				int entryFD = open(entryPath, O_RDONLY, 0666);
				char *fileTarget = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, targetFD, 0);
				char *fileEntry = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, entryFD, 0);
				//printf("fileTarget:\n-----------------------------------------------------\n%s", fileTarget);
				//printf("fileEntry:\n-----------------------------------------------------\n%s", fileEntry);
				if (!(strcmp(fileTarget, fileEntry))) {
					printf("SAME FILE CONTENTS WOOOOOOOOOOOO\n");
				}
				close(entryFD);
			}
		}
	}
	
	if (closedir(currentDir) == -1) {
		printf("PRINT ERROR MESSAGE!");
		printf("Error #: %d, String Error: %s", errno, strerror(errno));
	}
}

int readFile () {

}
