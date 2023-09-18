#include <stdio.h>

int main() {
	unsigned int a = 0x456;
	unsigned int b = ~(0xFF);
	unsigned int c = a & b;
	printf("Int a: %x, %d\nInt b: %x,%d\nInt c: %x, %d", a, a, b, b, c, c);
	return 0;
}
