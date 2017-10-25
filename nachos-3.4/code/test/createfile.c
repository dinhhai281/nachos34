#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main() {
	const int MAX_FILE_NAME_SIZE = 260;
	char fileName[MAX_FILE_NAME_SIZE];
	// Yeu cau nguoi dung nhap ten file
	Read(fileName, MAX_FILE_NAME_SIZE, 0);
	Create(fileName);
	Halt();
	return;
}