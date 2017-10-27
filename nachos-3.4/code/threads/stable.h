#include "bitmap.h"
#include "sem.h"
#define MAX_SEMAPHORE 10

class STable {
private:
	BitMap* bm;
	Sem* sTab[MAX_SEMAPHORE];
	int size;

public:
	STable(int size);
	~STable();

	int Create(char* name, int init);
	int Wait(char* name);
	int Signal(char* name);
	int FindFreeSlot();
};