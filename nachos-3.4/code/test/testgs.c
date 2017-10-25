#include "syscall.h"

int main() {
	char name[40];
	Write(name, 20, 1);

	PrintStr(name);

	Halt();
	return 0;
}