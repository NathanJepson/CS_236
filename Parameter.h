#pragma once
#include <vector>
#include <string>
#include <sstream>
using namespace std;

enum ParamType
{
	PARAM_STRING,
	PARAM_ID,
	PARAM_EXPRESSION
};

class Parameter
{
private:
	ParamType type;
	string value;
	bool isConstant;

public:
	Parameter(string & aValue)
	{
		this->value = aValue;
		if (value == "")
		{
			type = PARAM_ID;
		}
		else
		{
			char s = value.at(value.size() - 1);
			if ((string(1, value.at(0))) == "'" &&  string(1, s) == "'")
			{
				type = PARAM_STRING;
				isConstant = true;
			}
			else if (value.find("*") || value.find("+"))
			{
				type = PARAM_EXPRESSION;
				isConstant = false;
			}
			else
			{
				isConstant = false;
				type = PARAM_ID;
			}
		}
	}
	~Parameter() {}

	const string getValue()
	{
		return value;
	}
	const ParamType getType()
	{
		return type;
	}
	const bool Is_Constant()
	{
		return isConstant;
	}
};