#include "syscall.h"

int main() {
	int re;
	char c[2];
	c[1] = '\0';
	re = Seek(12, 1);
	if(re == -1) {

	}
	Write("Seek Console\n", 50, 1);
	Halt();
	return 0;
}