#pragma once
#include "Predicate.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Rule
{
private:
	Predicate head;
	vector<Predicate> body;

public:
	Rule()
	{
	}
	~Rule() {}
	void SetVals(Predicate & thisHead, vector<Predicate> & thisBody)
	{
		head = thisHead;
		body = thisBody;
	}
	Predicate & Get_Head()
	{
		return head;
	}
	vector<Predicate> & Get_Body()
	{
		return body;
	}
	string to_string()
	{
		ostringstream oss;
		oss << head.to_string() << " :- ";
		for (unsigned i = 0; i < body.size(); i++)
		{
			oss << body.at(i).to_string();
			if (i != (body.size() - 1))
			{
				oss << ",";
			}
		}
		oss << ".";
		return oss.str();
	}
};