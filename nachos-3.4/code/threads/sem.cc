#include "sem.h"

Sem::Sem(char* name, int value) {
	strcpy(this->name, name);
	sem = new Semaphore(this->name, value);
}

Sem::~Sem() {
	delete sem;
}

void Sem::Wait() {
	sem->P();
}

void Sem::Signal() {
	sem->V();
}

char* Sem::GetName() {
	return name;
}