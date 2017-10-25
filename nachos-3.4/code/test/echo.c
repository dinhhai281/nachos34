#include "syscall.h"

int main() {

	const int BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE];

	Write("\nEcho: ", BUFFER_SIZE, 1);
	Read(buffer, BUFFER_SIZE, 0);

	Write(buffer, BUFFER_SIZE, 1);
	Write("\n", 1, 1);


	Halt();
	return;
}