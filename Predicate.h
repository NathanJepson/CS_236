#pragma once
#include "Parameter.h"

using namespace std;


class Predicate
{
private:
	string id;
	vector<Parameter> params;


public:
	Predicate() {}
	~Predicate() {}
	string & Get_ID()
	{
		return id;
	}
	vector<Parameter> & Get_Params()
	{
		return params;
	}
	const string to_string()
	{
		ostringstream oss;
		oss << id << "(";
		for (unsigned i = 0; i < params.size(); i++)
		{
			oss << params.at(i).getValue();
			if (i != params.size() - 1)
			{
				oss << ",";
			}
		}
		oss << ")";
		return oss.str();
	}
	vector<string> Get_Vector_Params()
	{
		vector<string> result;
		for (unsigned i = 0; i < params.size(); i++)
		{
			result.push_back(params.at(i).getValue());
		}
		return result;
	}

	string String_Params()
	{
		vector<string> theseParams = Get_Vector_Params();
		string result = "";
		for (unsigned i = 0; i < theseParams.size(); i++)
		{
			result += theseParams.at(i);
			if (i != theseParams.size() - 1)
			{
				result += ",";
			}
		}
		return result;
	}

};