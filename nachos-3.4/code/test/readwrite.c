#include "syscall.h"

int main() {
	const int LENGTH = 50;
	int id;
	int byte;
	char buffer[LENGTH];



	id = Open("testcopy.txt", 0);
	byte = Write("abc", LENGTH, id);
	if(byte == 3 || byte == 4) {
		PrintStr("3 byteS");
	}
	if(byte == 50) {
		PrintStr("50");
	}
	Halt();
	return 0;
}