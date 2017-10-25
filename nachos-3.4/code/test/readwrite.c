#include "syscall.h"

int main() {
	const int LENGTH = 50;
	int id;

	char buffer[LENGTH];


	id = Open("testh.txt", 0);
	Read(buffer, LENGTH, id);
	Write(buffer, LENGTH, 1);
	Halt();
	return 0;
}