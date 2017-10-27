#include "syscall.h"

int main() {
	const int BUFFER_SIZE = 260;
	char buffer[BUFFER_SIZE];
	int id;

	Write("Enter file name: ", BUFFER_SIZE, 1);
	Read(buffer, BUFFER_SIZE, 0);

	id = Open(buffer, 1);

	Read(buffer, BUFFER_SIZE, id);
	Write(buffer, BUFFER_SIZE, 1);
	Write("\n", 1, 1);

	Halt();
	return 0;
}