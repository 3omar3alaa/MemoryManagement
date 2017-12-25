#include <iostream>
#include <vector>
#include <queue>
#include "Header.h"
using namespace std;

vector<process> processVector;
deque <process> processQueue;
vector<memory> memoryVector(10000);

int main()
{
	//memoryVector.reserve(10000);
	int quantum, switchTime, clk = 0;
	processVector = readProcess(quantum, switchTime);
	initializeMemory(memoryVector);
	bool switching = false;
	int currentProcessQuantum = quantum;
	int currentSwitchTime = switchTime;
	while (processVector.size() != 0 || processQueue.size() != 0)
	{
		//1- Check for any arrival of process
		while (processVector.size() > 0)
		{
			if (processVector.front().arrTime == clk)
			{
				processQueue.push_back(processVector.front());
				processVector.erase(processVector.begin());
			}
			else
				break;
		}
		//2- Check for current Process Queue if it has process & NOT switching now
		//Assign memory to process when it starts to run, not when it arrives
		if (processQueue.size() != 0 && !switching )
		{
			if (processQueue.front().memIndex != -1)
			{
				runProcess(processQueue, memoryVector, switching, currentProcessQuantum, quantum, clk);
			}
			else
			{
				if (assignMemory(processQueue.front(), memoryVector, 0))
				{
					runProcess(processQueue, memoryVector, switching, currentProcessQuantum, quantum, clk);
				}
				else
				{
					if (processQueue.front().back <= 5)
						processQueue.push_back(processQueue.front());
					processQueue.pop_front();
					cout << "process not assigned to memory address";
				}
			}
		}
	
		else if (switching)
		{
			if (currentSwitchTime == 0)
			{
				logSwitching(clk);
				switching = false;
				currentSwitchTime = switchTime;
			}
			else
			{
				currentSwitchTime--;
			}
		}
		clk++;
	}
	getResult(memoryVector);
	memoryVector.clear();
	processQueue.clear();
	processVector.clear();
	forOutput.clear();
	logEvent.close();
	return 0;
}