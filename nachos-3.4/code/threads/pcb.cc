#include "pcb.h"
#include "system.h"

extern void StartProcess_2(int id);


PCB::PCB(int id) {
	joinsem = new Semaphore("joinsem", 0);
	exitsem = new Semaphore("exitsem", 0);
	mutex 	= new Semaphore("mutex", 1);

	exitcode = 0;
	numwait = 0;
	pid = id;
	thread = NULL;
	parentID = currentThread->processID;
}

PCB::~PCB() {
	if(joinsem != NULL) {
		delete joinsem;
	}
	if(exitsem != NULL) {
		delete exitsem;
	}
	if(mutex != NULL) {
		delete mutex;
	}
	if(thread != NULL) {
		thread->Finish();
		delete thread;
	}
}

int PCB::Exec(char* filename, int pID) {
	mutex->P();
	// luu fileName vao filename
	strcpy(this->filename, filename);
	thread = new Thread(filename);
	// Kiem tra thread co duoc tao thanh cong
	if(thread == NULL) {
		mutex->V();
		return -1;
	}
	// Dat id cua thread nay la pID
	pid = pID;
	thread->processID = pid;
	parentID = currentThread->processID;
	thread->Fork(StartProcess_2, pid);
	mutex->V();
	return pid;
}

int PCB::GetID() {
	return pid;
}

int PCB::GetNumWait() {
	return numwait;
}

void PCB::JoinWait() {
	joinsem->P();
}

void PCB::ExitWait() {
	exitsem->V();
}

void PCB::JoinRelease() {
	joinsem->V();
}

void PCB::ExitRelease() {
	exitsem->V();
}

void PCB::IncNumWait() {
	mutex->P();
	numwait++;
	mutex->V();
}

void PCB::DecNumWait() {
	mutex->P();
	numwait--;
	mutex->V();
}

void PCB::SetExitCode(int ec) {
	exitcode = ec;
}

int PCB::GetExitCode() {
	return exitcode;
}

char* PCB::GetNameThread() {
	return filename;
}

void PCB::SetFileName(char* filename) {
	strcpy(this->filename, filename);
}

void PCB::SetParentID(int parentID) {
	this->parentID = parentID; 
}

int PCB::GetParentID() {
	return parentID;
}

int PCB::GetProcessID() {
	return pid;
}