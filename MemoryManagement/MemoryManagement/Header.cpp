#include "Header.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;


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
	memoryVector.push_back(m);
}
bool assignMemory(process &p,vector<memory> &memoryVector)
{
	int requiredMemory = nextPowerOf2(p.memSize + 6);
	//1- Loop to find if there is a place in the memory which matches the required space
	for (int i = 0; i < memoryVector.size(); i++)
	{
		if (memoryVector[i].size == requiredMemory && memoryVector[i].empty)
		{
			p.memIndex = i;
			memoryVector[i].empty = false;
			return true;
		}
	}
	//2- If there is no place in the memory which is equal to the desired one then create one
	int i = 0;
	while (memoryVector[i].size >= requiredMemory && memoryVector[i].empty)
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
		if (assignMemory(p, memoryVector))
			return true;
		i++;
	}
	return false;
}
void unAssignMemory(process p)
{

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
void runProcess(deque<process> &processQueue, bool&switching, int &currentProcessQuantum, int quantum)
{
	processQueue.front().remTime--;
	if (processQueue.front().remTime == 0)
	{
		unAssignMemory(processQueue.front());
		switching = true;
	}
	currentProcessQuantum--;
	if (currentProcessQuantum == 0)
	{
		switching == true;
		currentProcessQuantum = quantum;
	}
}