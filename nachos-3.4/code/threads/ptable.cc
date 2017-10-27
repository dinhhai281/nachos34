#include "ptable.h"
#include "system.h"

PTable::PTable(int size) {
	this->psize = size;
	this->bm = new BitMap(this->psize);
	this->bmsem = new Semaphore("bmsem", 1);

	for(int i = 0; i < this->psize; i++) {
		pcb[i] = NULL;
	}
	bm->Mark(0);
	pcb[0] = new PCB(0);
	pcb[0]->SetFileName(currentThread->getName());
	pcb[0]->SetParentID(-1);
}

PTable::~PTable() {
	for(int i = 0; i < this->psize; i++) {
		if(bm->Test(i) == true) {
			delete pcb[i];
		}
	}
	if(bmsem != NULL) {
		delete bmsem;
	}
	if(bm != NULL) {
		delete bm;
	}
}

int PTable::ExecUpdate(char* fileName) {
	OpenFile* file;
	bmsem->P();
	if(fileName == NULL) {
		bmsem->V();
		return -1;
	}

	file = fileSystem->Open(fileName);
	if(file == NULL) {
		bmsem->V();
		return -1;
	}

	if(strcmp(fileName, currentThread->getName()) == 0) {
		printf("\nPTable - Open itself");
		bmsem->V();
		return -1;
	}
	int slot = bm->Find();
	if(slot == -1) {
		printf("\nPTable - Not enough memory");
		bmsem->V();
		return -1;
	}

	pcb[slot] = new PCB(slot);
	pcb[slot]->SetFileName(fileName);
	pcb[slot]->SetParentID(currentThread->processID);
	int rc = pcb[slot]->Exec(fileName, slot);
	bmsem->V();
	return rc;
}

int PTable::ExitUpdate(int ec) {
	int id = currentThread->processID;
	if(id == 0) {
		interrupt->Halt();
		return 0;
	}

	if(bm->Test(id) == false) {
		printf("\nPTable - id not exists");
		return -1;
	}

	pcb[id]->SetExitCode(ec);
	pcb[pcb[id]->GetParentID()]->DecNumWait();
	pcb[id]->JoinRelease();
	pcb[id]->ExitWait();

}

int PTable::JoinUpdate(int id) {
	if(id == -1) {
		printf("\nPTable - JoinUpdate id is invalidi");
		return -1;
	}

	if(currentThread->processID != pcb[id]->GetParentID()) {
		printf("\nPTable - Join different thread");
		return -1;
	}

	pcb[pcb[id]->GetParentID()]->IncNumWait();
	pcb[id]->JoinWait();
	int ec = pcb[id]->GetExitCode();
	pcb[id]->ExitRelease();
	return ec;
}

void PTable::Remove(int id) {
	bmsem->P();
	if(bm->Test(id) == true) {
		delete pcb[id];
		pcb[id] = NULL;
		bm->Clear(id);
	}

	bmsem->V();
}

char* PTable::GetName(int id) {
	return pcb[id]->GetNameThread();
}