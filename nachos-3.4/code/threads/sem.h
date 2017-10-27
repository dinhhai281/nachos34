#include "synch.h"

class Sem {
private:
	char name[50];
	Semaphore* sem;

public:
	Sem(char* name, int value);
	~Sem();

	void Wait();
	void Signal();
	char* GetName();
};