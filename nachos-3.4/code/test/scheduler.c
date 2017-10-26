#include "syscall.h"

int main() {
	int pingPID, pongPID;
	Write("Ping - Pong test starting...\n\n", 100, 1);
	pongPID = Exec("./test/pong");
	pingPID = Exec("./test/ping");

	Join(pingPID);
	Join(pongPID);


	Halt();
	return 0;
}