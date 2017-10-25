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

// Tang program counter, code o trong file mipssim.cc
void IncreasePC() {
	machine->registers[PrevPCReg] = machine->registers[PCReg];
  	machine->registers[PCReg] = machine->registers[NextPCReg];
  	machine->registers[NextPCReg] += 4;
}

// -------------------------------------------------
// File preprocess
int fileIndex = 0;

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
					int i = 0;
					while(openFileList[i] != NULL) {
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
						printf("Failed to open file\n");
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
					printf("\n%d\n", id);
					// Kiem tra id
					if(idx > FILE_LIMIT) {
						printf("\n OpenFileID out of limit.");
						IncreasePC();
						machine->WriteRegister(2, -1);
						return;
					}
					if(openFileList[id] != NULL) { // file co dang duoc mo co the dong lai
						delete openFileList[id];
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