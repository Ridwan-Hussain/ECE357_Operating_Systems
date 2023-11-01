#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>

int main() {
	fprintf(stdout, "%d\n", SIGPIPE);
	return 0;
}
