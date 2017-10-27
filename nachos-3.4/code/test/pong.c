#include "syscall.h"

int main() {
	int i;
	for(i = 0; i < 1000; i++) {
		Down("pong");
		Write("B", 1, 1);
		Up("ping");
	}
	// PrintStr("For \n");
	// Up("ping");
	// Down("pong");
	// PrintStr("Horde \n");
	// Up("ping");
	return 1;
}