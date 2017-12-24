#include <iostream>
#include <vector>
#include <queue>
#include "Header.h"
using namespace std;

vector<process> processVector;
vector<memory> memoryVector;
deque <process> processQueue;
int main()
{
	int quantum, switchTime;
	processVector = readProcess(quantum, switchTime);
	initializeMemory(memoryVector);
	return 0;
}