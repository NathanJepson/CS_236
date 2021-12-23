#pragma once
#include <set>
//#include <fstream>
#include <sstream>

using namespace std;


class Node
{

private:
	set<int> predecessors;
	bool isVisited;
	int name;
	int postOrderNum;
	set<int> children;
public:
	Node(set<int> & givenAncestors);
	~Node();

	void Set_Flag()
	{
		isVisited = false;
	}
	void Trigger_Flag()
	{
		isVisited = true;
	}
	void Set_PostOrder(int & thisNumber)
	{
		postOrderNum = thisNumber;
	}
	int Get_PostOrder()
	{
		return postOrderNum;
	}
	string To_String();
	set<int> & getPredecessors()
	{
		return predecessors;
	}
	const bool hasBeenVisited()
	{
		return isVisited;
	}
	void Set_Name(int & thisName)
	{
		name = thisName;
	}
	const int Get_Name()
	{
		return name;
	}
	set<int> & getChildren()
	{
		return children;
	}
};

