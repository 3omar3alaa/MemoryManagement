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
			p.runTime = atoi(params.c_str());
		else if (i == 3)
			p.arrTime = atoi(params.c_str());
		else
			p.memSize = atoi(params.c_str());
		i++;
	}
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