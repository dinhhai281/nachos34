#include "ptable.h"
#include "system.h"

PTable::PTable(int size) {
	int i;
	psize = size;
	bm = new BitMap(psize);
	bmsem = new Semaphore("bmsem", 1);

	for(i = 0; i < psize; i++) {
		pcb[i] = NULL;
	}
	bm->Mark(0);
	pcb[0] = new PCB(0);
	pcb[0]->SetFileName("./test/scheduler");
	pcb[0]->SetParentID(-1);
}

PTable::~PTable() {
	int i;

	for(i = 0; i < psize; i++) {
		if(bm->Test(i)) {
			//delete pcb[i];
		}
	}

	if(bm != NULL) {
		delete bm;
	}
	if(bmsem != NULL) {
		delete bmsem;
	}
}

int PTable::ExecUpdate(char* filename) {
	int freeSlot;
	bmsem->P();
	// Kiem tra tinh hop le cua chuong trinh name
	if(filename == NULL) {
		printf("File name is NULL\n");
		bmsem->V();
		return -1;
	}

	OpenFile* of = fileSystem->Open(filename);
	if(of == NULL) { // file khong ton tai
		printf("File is not exist");
		bmsem->V();
		return -1;
	}
	// file co ton tai
	// kiem tra voi currentThread (currentThread la chuong trinh scheduler)
	if(strcmp(filename, "./test/scheduler") == 0) { //currentThread->getName()
		printf("Couldn\'t exec itself\n");
		bmsem->V();
		return -1;
	}

	if( strcmp(filename,currentThread->getName()) == 0 )
	{
		printf("\nPTable::Exec : Can't not exec itself");		
		bmsem->V();
		return -1;
	}

	// Tim slot trong trong bang PTable
	freeSlot = this->GetFreeSlot();
	if(freeSlot == -1) {
		bmsem->V();
		printf("\n PTable: Not enough memory for another thread");
		return -1;
	}
	pcb[freeSlot] = new PCB(freeSlot);
	pcb[freeSlot]->SetParentID(currentThread->processID);
	// khong can  phai danh dau lai vi ham Find trong class BitMap khi tim cung da danh dau roi
	int result =  pcb[freeSlot]->Exec(filename, freeSlot);
	bmsem->V();
	return result;
}

int PTable::ExitUpdate(int ec) {
	int id = currentThread->processID;
	if(id == 0) {
		// main process
		interrupt->Halt();
		return 0;
	}
	pcb[id]->SetExitCode(ec);
	// Goi JoinRelease de giai phong tien trinh cha
	pcb[pcb[id]->GetParentID()]->DecNumWait();
	pcb[id]->JoinRelease();
	pcb[id]->ExitWait();
	this->Remove(id);

	return ec;
}

int PTable::JoinUpdate(int id) {
	// Kiem tra tinh hop le
	if(id < 0) {
		printf("ID <  0\n");
		return -1;
	}
	// Kiem tra tien trinh goi join co phai la cha cua tien trinh <id> khong
	// Tien trinh goi join la tien trinh dang chay (currentThread)
	if(currentThread->processID != pcb[id]->GetParentID()) {
		printf("\nCouldn\'t join to thread\n");
		return -1;
	}
	// tang numwait cho tien trinh cha de tien trinh cha phai doi den khi tien trinh con ket thuc thi moi thuc hien tiep
	//int parentID = pcb[pcb[id]->GetParentID()];
	pcb[pcb[id]->GetParentID()]->IncNumWait();
	pcb[id]->JoinWait();
	int ec = pcb[id]->GetExitCode();

	pcb[id]->ExitRelease();

	return ec;
}

int PTable::GetFreeSlot() {
	//return gPhysPageBitMap->Find();
	return bm->Find();
}

bool PTable::IsExist(int pID) {
	return bm->Test(pID);
}

void PTable::Remove(int pID) {
	bmsem->P();
	bm->Clear(pID);
	delete pcb[pID];
	pcb[pID] = NULL;
}

char* PTable::GetName(int pID) {
	return pcb[pID]->GetNameThread();
}

PCB* PTable::GetThread(int pID) {
	return pcb[pID];
}