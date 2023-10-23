#include <sys/signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void Shandler(int s) {
	fprintf(stderr,"Got a signal %d\n",s);
	*(int* ) 0 = 3; 
	//kill(getpid(), SIGSEGV);

}

void Uhandler(int s) {
	fprintf(stderr,"Got a signal %d\n",s);
	*(int*) 0 = 0;
	//kill(getpid(), SIGURG);
}

int main() {
	signal(SIGSEGV,Shandler);
	signal(SIGURG, Uhandler);
	//fprintf(stderr, "GETPID: %d", getpid());
	fprintf(stderr,"Having a nice day, so far\n");
	//kill(getpid(), SIGSEGV);
	*(int *) 0 = 3;
	//for(;;)
	fprintf(stderr,"Goodbye!\n");
	return 0;
}
