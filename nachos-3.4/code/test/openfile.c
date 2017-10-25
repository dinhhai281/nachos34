#include "syscall.h"
#include "copyright.h"

int main() {
	//int type;
	//char name[32];
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


	Halt();
	return 0;
}