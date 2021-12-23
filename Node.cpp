#include "Node.h"
#include <vector>

using namespace std;


Node::Node(set<int> & givenAncestors)
{
	predecessors = givenAncestors;
	isVisited = false;
	postOrderNum = -1;
}


Node::~Node()
{
}

string Node::To_String()
{
	ostringstream oss;
	set<int>::iterator it;
	vector<int> result;
	for (it = predecessors.begin(); it != predecessors.end(); it++)
	{
		result.push_back(*it);
	}
	for (unsigned i = 0; i < result.size(); i++)
	{
		oss << "R" << result.at(i);
		if (i != result.size() - 1)
		{
			oss << ",";
		}
	}
	return oss.str();
}

