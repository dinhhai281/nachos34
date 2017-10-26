// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

//void
//ExceptionHandler(ExceptionType which)
//{
//    int type = machine->ReadRegister(2);

//    if ((which == SyscallException) && (type == SC_Halt)) {
//	DEBUG('a', "Shutdown, initiated by user program.\n");
//   	interrupt->Halt();
//    } else {
//	printf("Unexpected user mode exception %d %d\n", which, type);
//	ASSERT(FALSE);
//    }
//}

// Define
#define MAX_FILE_LENGTH 32
#define FILE_LIMIT 		10
#define STDIN 			0
#define STDOUT 			1

// Tang program counter, code o trong file mipssim.cc
void IncreasePC() {
	machine->registers[PrevPCReg] = machine->registers[PCReg];
  	machine->registers[PCReg] = machine->registers[NextPCReg];
  	machine->registers[NextPCReg] += 4;
}

// -------------------------------------------------
// File preprocess
int fileIndex = 2; // 0: stdin, 1: stdout


void ExceptionHandler(ExceptionType which) {
	int type = machine->ReadRegister(2);
	switch(which) {
		case NoException:
			return;
		case SyscallException:
			switch(type) {
				// Bat cac system call tai day, 
					//danh sach cac system call co trong userprog/syscall.h
				case SC_Halt:
					DEBUG('a', "Shutdown, initiated by user program.\n");
					interrupt->Halt();
					break;

				case SC_PrintStr:
				{
					int virtAddr;
					char *str;

					virtAddr = machine->ReadRegister(4);
					str = machine->User2System(virtAddr, MAX_FILE_LENGTH + 1);
					printf(str);
					delete []str;
					IncreasePC();
					break;
				}
				/*
				Input: reg4 - fileName(char*)
				Output: reg2 -1: error, 0:success
				*/
				case SC_Create: // system call Create file
				{
					int virtAddr;
					char* fileName;

					DEBUG('a', "\n SC_Create call ...");
					DEBUG('a', "\n Reading virtual address of file name");

					// Dia chi cua fileName o thanh gi so 4
					virtAddr = machine->ReadRegister(4);
					DEBUG('a', "\n Reading file name");
					// Dung ham User2System de doc du lieu tu vung nho ao len vung nho cua he thong (bien con tro fileName o tren)
					fileName = machine->User2System(virtAddr/*Dia chi cua chuoi filename trong reg4*/,
											MAX_FILE_LENGTH + 1); // Doc MAX_FILE_LENGTH + 1 byte bat dau tu virtAddr
					if(fileName == NULL) {
						printf("\n Not enough memory in system");
						DEBUG('a', "\n Not enough memory in system");
						IncreasePC();
						machine->WriteRegister(2, -1);
						delete fileName;
						return;
					}

					DEBUG('a', "\n Finish reading file name.");

					if(!fileSystem->Create(fileName, 0)) {
						printf("\n Error create file '%s'", fileName);
						IncreasePC();
						machine->WriteRegister(2, -1);
						delete fileName;
						return;
					}

					// Success Create file
					IncreasePC();
					machine->WriteRegister(2, 0);

					delete fileName;
					return;
				}
				case SC_Open:
				{
					int virtAddr;
					char* name;
					int type;

					// Kiem tra bo nho tim fileIndex
					int i = 2;
					while(openFileList[i] != NULL & i<= FILE_LIMIT) {
						i++;
					}
					if(i <= FILE_LIMIT) {
						fileIndex = i;
					} else {
						printf("\n Not enough memory");
						DEBUG('a', "\n Not enough memory");
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}

					// Doc dia chi choi name trong reg4
					virtAddr = machine->ReadRegister(4);
					name = machine->User2System(virtAddr, MAX_FILE_LENGTH + 1);
					if(name == NULL) { // File khong ton tai
						printf("\n File's not exsist.");
						DEBUG('a', "\n File's not exsit");
						IncreasePC();
						machine->WriteRegister(2, -1);
						delete name;
						return;
					}
					// Doc type trong reg5
					type = machine->ReadRegister(5);

					// Thu mo file va luu vao mang openFileList
					OpenFile* of = fileSystem->Open(name);
					if(of != NULL) {
						printf("%s open in type %d\n", name, type);
						openFileList[fileIndex] = of;
						fileTypeList[fileIndex] = type;
						IncreasePC();
						machine->WriteRegister(2, fileIndex);
					} else {
						printf("\n File's not exsist.");
						IncreasePC();
						machine->WriteRegister(2, -1);
					}

					delete name;
					return;
					//
				}
				break;
				case SC_Close:
				{
					int id;
					id = machine->ReadRegister(4);
					// Kiem tra id
					if(id > FILE_LIMIT || id < 0) {
						printf("\n OpenFileID out of limit.");
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
					if(openFileList[id] != NULL) { // file co dang duoc mo co the dong lai
						delete openFileList[id];
						openFileList[id] = NULL;
						IncreasePC();
						machine->WriteRegister(2, 0);
						return;
					} else {
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
				}
				break;
				case SC_Read:
				{

					int virtAddr; //dia chi chua noi dung se gi vao
					int size; // so byte ghi vao file
					int id; // OpenFileID
					char* buffer; // bo dem chua noi dung

					virtAddr = machine->ReadRegister(4);
					size = machine->ReadRegister(5);
					id = machine->ReadRegister(6);

					// Kiem tra neu ghi vao console(STDOUT)
					if(id == STDOUT) { // Khong the doc tu STDOUT
						IncreasePC();
						machine->WriteRegister(2, -1); 
						return;
					}
						
					if(id == STDIN)  { 
						buffer = new char[size];
						int num = gSynchConsole->Read(buffer, size);
						// Chuyen ve vung nho user
						machine->System2User(virtAddr, num, buffer);
						delete []buffer;
						IncreasePC();
						machine->WriteRegister(2, num);
						return;
					}

					if(id >= 2) { // File
						if(openFileList[id] == NULL) { // File khong duoc mo
							IncreasePC();
							machine->WriteRegister(2, -1);
							return;
						}
						// Bat dau doc file luu lai vi tri con tro trong file
						int curPos = openFileList[id]->CurrentPos();
						// Kiem tra da dem cuoi file chua
						if(curPos == openFileList[id]->Length()) {
							IncreasePC();
							machine->WriteRegister(2, -2);
							return;
						} 
						// Doc file
						buffer = new char[size];
						int num = openFileList[id]->Read(buffer, size);
						// Chuyen tu vung nho he thong san user
						machine->System2User(virtAddr, num, buffer);
						IncreasePC();
						machine->WriteRegister(2, num);
						delete []buffer;
						return;


					}
				}
				break;
				case SC_Write: // Write(char* buffer, int size, OpenFileID id)
				{	
					int virtAddr; //dia chi chua noi dung se gi vao
					int size; // so byte ghi vao file
					int id; // OpenFileID
					char* buffer; // bo dem chua noi dung
					// Doc cac gia tri tu thanh ghi
					virtAddr = machine->ReadRegister(4);
					size = machine->ReadRegister(5);
					id = machine->ReadRegister(6);
					// Kiem tra neu ghi vao console(STDOUT)
					if(id == STDOUT) {
						buffer = machine->User2System(virtAddr, size);
						int num = gSynchConsole->Write(buffer, size);
						IncreasePC();
						machine->WriteRegister(2, num);
						delete []buffer;
						return;
					}
					if(id == STDIN)  { // Khong the Write vao STDIN
						IncreasePC();
						machine->WriteRegister(2, -1); 
						return;
					}
					if(id >= 2) {
						//Kiem tra OpenFileID
						if(openFileList[id] == NULL) {
							printf("\nOpenFileID is not valid");
							IncreasePC();
							machine->WriteRegister(2, -1);
							return;
						} else if(fileTypeList[id] == 1) { // type = 1 -> file chi doc
							printf("\nReadonly file");
							IncreasePC();
							machine->WriteRegister(2, -1);
							return;
						}
						// Chuyen buffer vao vung nho he thong
						buffer = machine->User2System(virtAddr, size);
						int realSize = 0;
						while(!(buffer[realSize] == 0 && buffer[realSize + 1] == 0) || realSize == size) {
							realSize++;
						}
						// Ghi vao file su dung OpenFile
						int num = openFileList[id]->Write(buffer, realSize - 1); // Tra ve so byte that su da ghi

						IncreasePC();
						machine->WriteRegister(2, num);
						delete []buffer;
						return;
					}
					

				}
				break;
				case SC_Seek:
				{
					int pos;
					int id;

					pos = machine->ReadRegister(4);	
					id 	= machine->ReadRegister(5);
					//Kiem tra file da duoc open hay chua
					if(openFileList[id] == NULL) {
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}

					// Kiem tra pos co lon hon do dai cua file
					if(pos > openFileList[id]->Length()) {
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
					// Kiem tra neu goi ham seek tren console IO
					if(id == 0 || id == 1) {
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
					// Kiem tra neu pos = -1 thi chuyen toi cuoi file
					if(pos == -1) {
						int lastByte = openFileList[id]->Length();
						openFileList[id]->Seek(lastByte);
						IncreasePC();
						machine->WriteRegister(2, lastByte);
						return;
					}

					// Truong hop binh thuong
					openFileList[id]->Seek(pos);
					IncreasePC();
					machine->WriteRegister(2, openFileList[id]->CurrentPos());
				}
				break;
				case SC_Exec:
				{
					

					int virtAddr;
					char* filename;

					virtAddr = machine->ReadRegister(4);
					filename = machine->User2System(virtAddr, 100);
					if(filename == NULL) {
						printf("\nKhong mo duoc file\n");
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
					int result = pTab->ExecUpdate(filename);
					machine->WriteRegister(2, result);
					delete []filename;
					IncreasePC();
				}
				break;
				case SC_Join:
				{	
					int id;
					id = machine->ReadRegister(4);
					int result = pTab->JoinUpdate(id);
					machine->WriteRegister(2, result);
				}
				break;
				case SC_Exit:
				{
					int exitStatus;
					exitStatus = machine->ReadRegister(4);
					int result = pTab->ExitUpdate(exitStatus);
					machine->WriteRegister(2, result);
				}	
				break;
				//case Other:
			}
			break;
		case PageFaultException:
			printf("\n No valid translation found");
			interrupt->Halt();
			break;

		case ReadOnlyException:
			printf("\n Write attempted to page marked \"read-only\"");
			interrupt->Halt();
			break;

		case BusErrorException:
			printf("\n Translation resulted in an invalid physical address");
			interrupt->Halt();
			break;

		case AddressErrorException:
			printf("\n Unaligned reference or on that was beyond the end of the address space");
			interrupt->Halt();
			break;

		case OverflowException:
			printf("\n Integer overflow in add or sub.");
			interrupt->Halt();
			break;

		case IllegalInstrException:
			printf("\n Unimplemented or reserved instruction");
			interrupt->Halt();
			break;
		default:
				printf("\n Unexpected user mode exception (%d %d)", which, type);
				interrupt->Halt();
	}
}