#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main() {
	int len;
	char fileName[maxlen + 1];

	if(Create("text.txt") == -1) {
		PrintStr("\nFailed on creating file\n");
	} else {
		PrintStr("\nSuccess\n");
	}
	Halt();
}