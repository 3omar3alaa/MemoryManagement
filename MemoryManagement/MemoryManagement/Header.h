#include <string>
#include <vector>
using namespace std;
#pragma once


struct memory
{
	int start;
	int end;
	int size;
	bool empty;
};

struct process 
{
	int arrTime;
	int runTime;
	int memSize;
	int back; //This is to count how many times the process went back to the end of the queue when there is no space
	int memIndex;
	int finishTime;
	int remTime;
	string id;
};

vector <process> readProcess(int &Quantum, int &switchTime);
process buildProcess(string line);
void initializeMemory(vector<memory> &memoryVector);