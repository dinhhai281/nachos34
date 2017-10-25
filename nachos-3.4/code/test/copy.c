#include "syscall.h"

int main() {

	const int MAX_FILE_LENGTH = 260;
	const int BUFFER_SIZE = 1024;


	char src[MAX_FILE_LENGTH];
	int srcId;
	int srcSize;
	char des[MAX_FILE_LENGTH];
	int desId;
	int desSize;

	char buffer[BUFFER_SIZE];

	Write("Enter source file: ", BUFFER_SIZE, 1);
	Read(src, MAX_FILE_LENGTH, 0);

	Write("Enter destination file: ", BUFFER_SIZE, 1);
	Read(des, MAX_FILE_LENGTH, 0);

	// Doc noi dung file nguon vao buffer
	srcId = Open(src, 1);
	if(srcId == -1 || srcId == 0 || srcId == 1) { // Cac truong hop ngoai le
		Write("Denided\n", BUFFER_SIZE, 1);
		Halt();
		return;
	}
	// lay kich thuoc file src
	srcSize = Read(buffer, BUFFER_SIZE, srcId);
	// tao file va ghi noi dung file doc duoc vao dia diem dich
	Create(des);
	desId = Open(des, 0);
	// Ghi noi dung
	desSize = Write(buffer, BUFFER_SIZE, desId);


	Halt();
	return 0;
}