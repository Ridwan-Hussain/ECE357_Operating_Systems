#include <unistd.h>

int main() {
	char buf[1];
	int n;
	while ((n=read(0, buf, 1))>0) {
		if (buf[0] =='\n')
			write(1, "\r", 1);
		write(1, buf,1);
	}

	return 0;
}
