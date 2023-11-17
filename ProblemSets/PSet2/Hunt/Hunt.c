// Ridwan Hussain - Program 2 Hunt
// Used pubs.opengroup.org opendir, readdir, stat, and other various examples
// as references while making the program.
// Jacob Koziej (EE'25) also gave me advice on using mmap 
// Also referenced https://www.youtube.com/watch?v=m7E9piHcfr4
#define MAXPATHNAMELENGTH 4096

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

void searchDir(char* pathName, char* target, int fileSize, int targetIno, int targetFD);
void fileContentMatch(char* entryPath, struct stat statBuf, int targetFD, int fileSize, int symLink);

int main(int argc, char *argv[]) {
	//Syntax is ./hunt <targetPath> <startPath>
	char* target = argv[1];
	char *startPath = argv[2];
	struct stat statBuf;
	stat(target, &statBuf);
	int fileSize = statBuf.st_size;
	int targetIno = statBuf.st_ino;
	
	//CHECK FOR ERROR HERE
	int targetFD = open(target, O_RDONLY, 0666);
	//

	searchDir(startPath, target, fileSize, targetIno, targetFD);
	close(targetFD);

	return 0;
}

void searchDir(char* pathName, char* target, int fileSize, int targetIno, int targetFD) {
	DIR *currentDir;
	struct dirent *dirEntries;
	if ((currentDir = opendir(pathName)) == NULL) {
		printf("Can't Open %s, Permission Denied\n", pathName);
		return;
	}
	char entryPath[MAXPATHNAMELENGTH];
	struct stat statBuf, statLinkBuf;
	while ((dirEntries = readdir(currentDir)) != NULL) {
		if (!(strcmp(dirEntries->d_name, ".")) || !(strcmp(dirEntries->d_name, ".."))) {
			continue;
		}
		//EntryPath is the PathName + / + fileName
		strcpy(entryPath, pathName);
		strcat(entryPath, dirEntries->d_name);
		if (stat(entryPath, &statBuf) == -1 || lstat(entryPath, &statLinkBuf) == -1) {
			printf("Could not stat the item: %s.\n", entryPath);
			printf("Error #: %d, String Error: %s\n", errno, strerror(errno));
		} else if (S_ISDIR(statBuf.st_mode)) { //Check if entry is a directory
			strcat(entryPath, "/");
			if (statBuf.st_mode && (S_IRUSR || S_IXUSR)) {
				searchDir(entryPath, target, fileSize, targetIno, targetFD);
			} else {
				printf("Do not have proper permissions for opening file directory.\n");
			}
		//Check for read permissions, assume person running the code is user
		} else if (!(statBuf.st_mode && S_IRUSR)) {
			printf("Do not have proper permissions for %s\n", entryPath);
		} else if (S_ISLNK(statLinkBuf.st_mode)) {
				if (!(S_ISREG(statBuf.st_mode))){
					printf("Symlink, %s, does not point to file, so it was skipped.\n", entryPath);
				} else if ((statBuf.st_mode && statLinkBuf.st_mode && S_IRUSR) && (statBuf.st_size == fileSize)) {
					fileContentMatch(entryPath, statBuf, targetFD, fileSize, 1);
				}
		} else if (S_ISREG(statBuf.st_mode)) {
			if (statBuf.st_size == fileSize) {
				if ((statBuf.st_ino == targetIno)) {
					if (!strcmp(entryPath, target)) {
						printf("Found original Target File!\nPathname: %s\n\n", entryPath);
					} else {
						printf("Pathname: %s\nHARD LINK to Target\nFile has %d nlinks\n\n", entryPath, statBuf.st_nlink);
					}
				} else {
					fileContentMatch(entryPath, statBuf, targetFD, fileSize, 0);
				}
			}
		} else {
			printf("Item, %s, is not a directory, file, or link, so it was skipped.\n", entryPath);
		}
	}
	if (closedir(currentDir) == -1) {
		printf("Error Closing Dir, Error #: %d, String Error: %s\n", errno, strerror(errno));
	}
}

void fileContentMatch(char* entryPath, struct stat statBuf, int targetFD, int fileSize, int symLink) {
	int entryFD = open(entryPath, O_RDONLY, 0666);
	char *fileTarget = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, targetFD, 0);
	char *fileEntry = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, entryFD, 0);
	if (!(strcmp(fileTarget, fileEntry))) {
		printf("Pathname: %s\nFile MATCHES Target", entryPath);
		if (statBuf.st_nlink > 1) {
			printf("\nFile has %d nlinks", statBuf.st_nlink);
		}
		if (symLink) {
			printf("\nFile was a SymLink\n\n");
		} else {
			printf("\n\n");
		}
	} 
	close(entryFD);
}
