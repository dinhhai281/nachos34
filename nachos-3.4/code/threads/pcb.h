#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

#define MAX_FILE_LENGTH 64

class PCB
{
private:
	Semaphore	*joinsem;	//semaphore cho qua trinh join
	Semaphore	*exitsem;	//semaphore cho qua trinh exit
	Semaphore	*mutex;
	int		exitcode;
	int		numwait;	//so tien trinh da join
	int		pid;
	char filename[MAX_FILE_LENGTH];
	int 		parentID;	//ID cua tien trinh cha
	
public:
	Thread		*thread;
	
	PCB(int id);
	~PCB();
	int Exec(char *filename, int pID); //nap chuong trinh co ten luu trong bien filename va processID se la pID
	int GetID();
	int GetNumWait();
	void JoinWait();
	void ExitWait();
	void JoinRelease();
	void ExitRelease();
	void IncNumWait();
	void DecNumWait();
	void SetExitCode(int ec);
	int GetExitCode();
	char* GetNameThread(); // filename
	void SetFileName(char* filename);
	void SetParentID(int parentID);
	int GetParentID();
	int GetProcessID();
};

//*********************************************************************************************************************
void MyStartProcess(int pID);

#endif