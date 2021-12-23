#pragma once

#include "Rule.h"
#include "Database.h"
#include "Node.h"
#include <iostream>
#include <fstream>
#include <stack> 
#include <stack> 


using namespace std;


class Interpreter
{
private:
	struct dataProgram
	{
		vector<Predicate> schemesS;
		vector<Predicate> factsS;
		vector<Rule> rulesS;
		vector<Predicate> queriesS;
		vector<string> domainS;
	};

	int iteratorT;

	vector<dataProgram> programsQ;
	Database myDatabase;
	//vector<Database> databases;
	ostringstream output;
	
	//////////////////////////////////////
	set<Tuple> addedTuples;

	vector<Relation> recentRelation;

	struct loggs
	{
		string name9;
		Relation itsRelation;
		Rule itsRule;
	};

	map<int, loggs>  ss;
	int ssIterator;
	vector<map<int, loggs>> final;

	//////////////////////////////////////
	set<int> SCCtracker;
	set<int> SCCtracker2;
	int SCCtracker3;
	vector<set<int>> SCCs;
	
	Database pastLogger;
	int Debugger;

public:
	Interpreter();
	~Interpreter();

	Database & Get_DB(int & where);
	void Write_Database();
	void Evaluate_Queries(int location);
	
	
	//Relation Evaluate_Single_Query(Relation aRelation, string queryString);
	unsigned Evaluate_Rules(vector<Rule> & theLaws);
	void Acquire_DatalogProgram(vector<Predicate> schemesQ, vector<Predicate> factsQ,
		vector<Rule> rulesQ, vector<Predicate> queriesQ, vector<string> domainQ);
	string convVectorString(vector<string> & theseParams);

	unsigned Fixed_Point_Algorithm(vector<Node*> thisSCC, vector<Rule> ruleList, vector<Node*> allNodes);

	string String_Params_2(vector<string> & thisVector);

	void WriteToConsole()
	{
		
		ofstream ofs;
		ofs.open("out_file.txt", std::ofstream::out | std::ofstream::trunc); //erases contents of parsed.txt
		ofs.close();
		ofs.open("out_file.txt");
		ofs << output.str();
		ofs.close();
		

		cout << output.str();
	}
	void Evaluate_Graph(int location);
	set<int> traverseGraph(vector<Node*> & precedenceList, vector<Node*> & nodeList, int toVisit);
	
	//void Evaluate_New_Tuples();
	map<int,Node*> Get_Post_Order(map<int,Node*> moreNodes, int whichNode, stack<int> & helpStack);
	map<int, Node*> Post_Order_Helper(map<int, Node*> moreNodes, int targetNode, stack<int> & helpStack);
};

//#endif