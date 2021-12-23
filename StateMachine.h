#pragma once
#include <string>
#include <map>
//#include <unordered_map>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

class StateMachine
{

private:
	int lineNum;
	int tempLineCount;
	ostringstream output;
	string tempWhatIs;
	//int tokens;
	int debug;
	vector<string> lexedTokens;

public:
	void Insert(string type, string whatIs);
	void AddLineNum();
	void StateZero(char temp, ifstream &theFile);
	void StateOne(char temp, ifstream &theFile);
	void StateTwo(char temp, ifstream &theFile);
	void StateThree(char temp, ifstream &theFile);
	void StateFour(char temp, ifstream &theFile);
	void StateFive(char temp, ifstream &theFile);
	void StateSix(char temp, ifstream &theFile);
	void StateSeven(char temp, ifstream &theFile);
	void StateEight(char temp, ifstream &theFile);
	vector<string> GetOutput();
	StateMachine();
	~StateMachine();
	int GetDebug();
};