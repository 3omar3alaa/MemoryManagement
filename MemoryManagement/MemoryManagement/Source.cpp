#include <iostream>
#include <vector>
#include <queue>
#include "Header.h"
using namespace std;

vector<process> processVector;
vector<memory> memoryVector(10000);
deque <process> processQueue;
int main()
{
	//memoryVector.reserve(10000);
	int quantum, switchTime, clk = 0;
	processVector = readProcess(quantum, switchTime);
	initializeMemory(memoryVector);
	bool switching = false;
	int currentProcessQuantum = quantum;
	int currentSwitchTime = switchTime;
	while (processVector.size() != 0)
	{
		//1- Check for current Process Queue if it has process & NOT switching now
		if (processQueue.size() != 0 && !switching)
		{
			if (processQueue.front().memIndex == -1)
			{
				if (assignMemory(processQueue.front(),memoryVector,0))
				{
					runProcess(processQueue, memoryVector,switching, currentProcessQuantum,  quantum);
				}
				else
				{
					cout << "process not assigned to memory address";
				}
			}
			else
			{
				runProcess(processQueue, memoryVector, switching, currentProcessQuantum, quantum);
			}
		}
		//2- Check for any arrival of process
		while (processVector.front().arrTime == clk)
		{
			processQueue.push_back(processVector.front());
			processVector.erase(processVector.begin());
			if (!assignMemory(processQueue.front(), memoryVector,0))
			{
				processQueue.push_back(processQueue.front());
				processQueue.pop_front();
			}
		}
		if (switching)
		{
			currentSwitchTime--;
		}
		clk++;
	}
	return 0;
}