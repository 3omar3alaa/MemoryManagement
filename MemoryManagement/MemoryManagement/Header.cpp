#include "Header.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

int lastEventTime;
ofstream logEvent;

vector<process> readProcess(int &Quantum, int &switchTime)
{
	vector <process> processVector;
	ifstream in("input.txt");
	string line;
	string params;
	int lineCount = 1;
	while (getline(in,line))
	{
		if (lineCount > 3)
		{
			processVector.push_back(buildProcess(line));
		}
		else if (lineCount == 1)
		{
			stringstream iss(line);
			while (getline(iss, params, ' '))
			{
				if (params != "Quantum")
				{
					Quantum = atoi(params.c_str());
				}
			}
		}
		else if (lineCount == 2)
		{
			stringstream iss(line);
			while (getline(iss, params, ' '))
			{
				if (params != "Switch")
				{
					switchTime = atoi(params.c_str());
				}
			}
		}
		lineCount++;
	}
	in.close();
	return processVector;
}

process buildProcess(string line)
{
	process p;
	string params;
	stringstream iss(line);
	int i = 1;
	while (getline(iss, params, ' '))
	{
		if (i == 1)
			p.id = params;
		else if (i == 2)
			p.runTime = p.remTime = atoi(params.c_str());
		else if (i == 3)
			p.arrTime = atoi(params.c_str());
		else
			p.memSize = atoi(params.c_str());
		i++;
	}
	p.memIndex = -1;
	return p;
}
void initializeMemory(vector<memory> &memoryVector)
{
	memory m;
	m.empty = true;
	m.size = 1024;
	m.start = 0;
	m.end = m.size - 1;
	memoryVector[0] = m;
}
bool assignMemory(process &p,vector<memory> &memoryVector,int index)
{
	int minMemoryIndex = index;
	int requiredMemory = nextPowerOf2(p.memSize + 6);
	//1- Loop to find if there is a place in the memory which matches the required space
	for (int i = index; i < memoryVector.size(); i++)
	{
		if (memoryVector[i].size == requiredMemory && memoryVector[i].empty)
		{
			p.memIndex = i;
			memoryVector[i].empty = false;
			return true;
		}
		if (memoryVector[i].size > requiredMemory && memoryVector[i].empty && memoryVector[minMemoryIndex].size > memoryVector[i].size && memoryVector[i].size != 0)
		{
			minMemoryIndex = i;
		}
		if (memoryVector[i].size < requiredMemory && memoryVector[i].size != 0)
		{
			break;
		}
	}
	//2- If there is no place in the memory which is equal to the desired one then create one
	int i = minMemoryIndex;
	while (memoryVector[i].size > requiredMemory && memoryVector[i].empty && i < memoryVector.size())
	{
		memory m1,m2;
		m1.size = m2.size = memoryVector[i].size / 2;
		m1.empty = m2.empty = true;
		m1.start = memoryVector[i].start;
		m1.end = m1.start + m1.size - 1;
		m2.start = memoryVector[i].start + m2.size;
		m2.end = memoryVector[i].end;
		memoryVector[2 * i + 1] = m1;
		memoryVector[2 * i + 2] = m2;
		memoryVector[i].empty = false;
		if (assignMemory(p, memoryVector, 2 * i + 1))
			return true;
		i++;
	}
	p.back++;
	return false;
}
void unAssignMemory(int memIndex, vector<memory> &memoryVector, bool check)
{
	if (memIndex == 0)
		return;
	if (!check)
		memoryVector[memIndex].empty = true;
	int parentIndex;
	if (memIndex % 2 == 0)
	{
		parentIndex = (memIndex - 2) / 2;
		if (memoryVector[2 * parentIndex + 1].empty)
		{
			memoryVector[parentIndex].empty = true;
			memoryVector[memIndex].size = 0;
			memoryVector[2 * parentIndex + 1].size = 0;
			unAssignMemory(parentIndex, memoryVector,true);
		}
	}
	else
	{
		parentIndex = (memIndex - 1) / 2;
		if (memoryVector[2 * parentIndex + 2].empty)
		{
			memoryVector[parentIndex].empty = true;
			memoryVector[memIndex].size = 0;
			memoryVector[2 * parentIndex + 2].size = 0;
			unAssignMemory(parentIndex, memoryVector,true);
		}
	}
}
int nextPowerOf2(int n)
{
	unsigned int p = 1;
	if (n && !(n & (n - 1)))
		return n;

	while (p < n)
		p <<= 1;

	return p;
}
void runProcess(deque<process> &processQueue, vector<memory> &memoryVector,bool&switching, int &currentProcessQuantum, int quantum, int clk)
{
	if (processQueue.front().remTime > 0 && currentProcessQuantum > 0)
	{
		if (currentProcessQuantum == quantum) {
			lastEventTime = clk;
			//assuming printing queue at start of executing a process
			printProcessQueue(processQueue);
		}
			
		processQueue.front().remTime--;
		currentProcessQuantum--;
	}
	else if (processQueue.front().remTime == 0)
	{
		logProcessFinish(memoryVector, processQueue, clk);
		processQueue.front().finishTime = clk;
		lastEventTime = clk + 1;
		unAssignMemory(processQueue.front().memIndex, memoryVector,false);
		forOutput.push_back(processQueue.front());
		processQueue.pop_front();
		currentProcessQuantum = quantum;
		switching = true;
		return;
	}
	else
	{
		//assuming that we reset the quantum if no process in queue
		currentProcessQuantum = quantum;
		if (processQueue.size() > 1) {
			logProcessStop(memoryVector, processQueue, clk);
			lastEventTime = clk + 1;
			switching = true;
			processQueue.push_back(processQueue.front());
			processQueue.pop_front();
		}		
	}
}

void printProcessQueue(deque<process> &p) {
	logEvent << "Queue: ";
	for (int i = 0; i < p.size(); i++) {
		logEvent << p.at(i).id;
		if (i < p.size() - 1)
			logEvent << ", ";
	}

	logEvent << endl;
}

void logProcessStop(vector<memory> &memVector, deque<process> &p, int clk) {
	logEvent << "Executing process " << p.front().id << "\t: started at " << lastEventTime
		<< ", stopped at " << clk << ", "<< p.front().remTime <<" remaining, memory starts at " << memVector[p.front().memIndex].start
		<< " and ends at " << memVector[p.front().memIndex].end << endl;
}

void logProcessFinish(vector<memory> &memVector, deque<process> &p, int clk) {
	logEvent << "Executing process " << p.front().id << "\t: started at " << lastEventTime
		<< ", finished at " << clk << ", memory starts at " << memVector[p.front().memIndex].start
		<< " and ends at " << memVector[p.front().memIndex].end << endl;
}

void logSwitching(int clk) {
	logEvent << "Process switching\t: started at " << lastEventTime << ", finished at " << clk << endl;
}


bool sortProcessesById(process i, process j) { 
	return i.id < j.id; 
}

void getResult(vector<memory> &memVector) {
	sort(forOutput.begin(), forOutput.end(), sortProcessesById);
	ofstream out("output.txt");
	out << "process_id\t";
	out << "run_time\t";
	out << "arrival_time\t";
	out << "finish_time\t";
	out << "mem_size\t";
	out << "mem_start\t";
	out << "mem_end\n";
	for (int i = 0; i < forOutput.size(); i++) {
		out << forOutput[i].id << "\t";
		out << forOutput[i].runTime << "\t";
		out << forOutput[i].arrTime << "\t";
		out << forOutput[i].finishTime << "\t";
		out << forOutput[i].memSize << "\t";
		out << memVector[forOutput[i].memIndex].start << "\t";
		out << memVector[forOutput[i].memIndex].end << "\n";
	}
	out.close();
}

void logNoMemorySpace(deque<process> &p) {
	printProcessQueue(p);
	logEvent << "No space for " << p.front().id << endl;
}