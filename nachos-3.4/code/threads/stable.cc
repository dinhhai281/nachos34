#include "stable.h"

STable::STable(int size) {
	this->size = size;
	bm = new BitMap(this->size);

	for(int i = 0; i < this->size; i++) {
		sTab[i] = NULL;
	}
}

STable::~STable() {
	delete bm;
	for(int i = 0; i < this->size; i++) {
		if(sTab[i] != NULL) {
			delete sTab[i];
			sTab[i] = NULL;
		}
	}
}

int STable::Create(char* name, int value) {
	int freeSlot = this->FindFreeSlot();
	if(freeSlot == -1) {
		printf("\nNot enough memory");
		return -1;
	}
	sTab[freeSlot] = new Sem(name, value);
	return 0;
}

int STable::Wait(char* name) {
	for(int i = 0; i < this->size; i++) {
		if(sTab[i] != NULL) {
			if(strcmp(sTab[i]->GetName(), name) == 0) {
				sTab[i]->Wait();
				return 0;
			}
		}
		
	}
	return -1;
}

int STable::Signal(char* name) {
	for(int i = 0; i < this->size; i++) {
		if(sTab[i] != NULL) {
			if(strcmp(sTab[i]->GetName(), name) == 0) {
				sTab[i]->Signal();
				return 0;
			}
		}
	}

	return -1;
}

int STable::FindFreeSlot() {
	return bm->Find();
}