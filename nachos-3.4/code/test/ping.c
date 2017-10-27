#include "syscall.h"

int main() {
	int i;
	for(i = 0; i < 1000; i++) {
		Write("A", 1, 1);
		Up("pong");
		Down("ping");

	}
	//PrintStr("Up ping\n");
	// Down("ping");
	// PrintStr("The \n");
	// Up("pong");
	// Down("ping");
	
	return 1;
}