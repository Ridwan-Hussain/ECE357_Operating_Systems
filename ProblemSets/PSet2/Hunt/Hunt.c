// Ridwan Hussain - Program 2 hunt
// Used pubs.opengroup.org opendir, readdir, stat, and other various examples
// as references while making the program.
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* Program Checklist
 * 1. Get File Name, store file bytes and file contents
 * 2. opendir, readdir, closedir, fprintf, strlen
 */

int main(int argc, char *argv[]) {
	
	return 0;
}

int searchDir(char* dirName, char* pathName) {
	// Code was referenced from opendir and readdir pages on pubs.opengroup.org
	DIR *currentDir;
	struct dirent *dirEntries;
	if ((currentDir = opendir(dirName)) == NULL) {
		printf("Current Dir - %s - is NULL. Print Error Message.\n", dirName);
		exit(1);
	}
	char* entryPath;
	struct stat statbuffer;
	int status;
	while ((dirEntries == readdir(currentDir)) != NULL) {
		entryPath = strcat(pathName, dirEntries);
		//Might need to replace `dirEntries->d_name` with `entryPath`
		if (stat(dirEntries->d_name, &statBuffer) == -1) {
			printf("Could not find the file.\n");
			exit(2);
		}
		// if we have good permissions, assume person running the code is user
		//permissions = S_IRUSR, use && 
		else if (sperm(statBuffer.st_mode) != permissions ) {
			printf("Do not have proper permissions\n");
			exit(3);
		} else if () {

		}
	}

	/*opendir*/
	
	/*store directory contents/items */
		/* if dir, searchDir(dirName, pathName) */
		/* if file, fileMatch() */

	/* close directory */
}
