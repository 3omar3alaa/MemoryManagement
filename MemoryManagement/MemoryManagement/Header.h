#include <string>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;
#pragma once

static int lastEventTime;
static ofstream logEvent("log.txt");

struct memory
{
	int start;
	int end;
	int size;
	bool empty;
};

struct process 
{
	string id;
	int arrTime;
	int runTime;
	int memSize;
	int back; //This is to count how many times the process went back to the end of the queue when there is no space
	int memIndex = -1;
	int finishTime;
	int remTime;
};
static vector<process> forOutput;

vector <process> readProcess(int &Quantum, int &switchTime);
process buildProcess(string line);
void initializeMemory(vector<memory> &memoryVector);
bool assignMemory(process &p,vector<memory> &memoryVector,int index);
void unAssignMemory(int memIndex, vector<memory> &memoryVector, bool check);
void runProcess(deque<process> &processQueue, vector<memory> &memoryVector, bool&switching, int &currentProcessQuantum, int quantum, int clk);
int nextPowerOf2(int n);
void logProcessFinish(vector<memory> &memVector, deque<process> p, int clk);
void logProcessStop(vector<memory> &memVector, deque<process> p, int clk);
void printProcessQueue(deque<process> p);
void logSwitching(int clk);
void getResult(vector<memory> &memVector);