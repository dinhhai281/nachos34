#include "syscall.h"

int main() {
int i;
	char m[2];

	if(Create("test.txt") == 0) {
	 	PrintStr("\ntest.txt created\n");
	}
	if(Create("test1.txt") == 0) {
	 	PrintStr("\ntest1.txt created\n");
	}
	if(Create("test2.txt") == 0) {
		PrintStr("\ntest2.txt created\n");
	}
	if(Create("test3.txt") == 0) {
		PrintStr("\ntest3.txt created\n");
	}

	
	m[0] = Open("test.txt", 1);
	m[0] += 48;
	m[1] = '\0';
	PrintStr(m);
	PrintStr("\n");

	m[0] = Open("test1.txt", 1);
	m[0] += 48;
	m[1] = '\0';
	PrintStr(m);
	PrintStr("\n");


	m[0] = Open("test2.txt", 1);
	m[0] += 48;
	m[1] = '\0';
	PrintStr(m);
	PrintStr("\n");

	m[0] = Open("test3.txt", 1);
	m[0] += 48;
	m[1] = '\0';
	PrintStr(m);
	PrintStr("\n");

	Close(0);
	Close(1);
	Close(2);
	Close(3);
	Halt();
	return;
}