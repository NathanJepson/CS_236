#pragma once
#include "Rule.h"

#include <fstream>

using namespace std;

class DatalogProgram
{
private:
	struct valueLineNum
	{
		string value;
		string lineNum;
	};

	vector<string> input;
	vector<string> tokenTypes;
	vector<valueLineNum> endTokens;

	bool isSchemes;
	bool isFacts;
	bool isRules;
	bool isHead;
	bool isBody;
	bool isQueries;


	bool isValid;


	bool inExpression;

	ostringstream output;
	string offendingToken;

	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	vector<string> domain;

	string recentWhatIs;

public:
	DatalogProgram(vector<string> tokens);
	~DatalogProgram();
	void Parse();

	bool Match(string tokenType);
	bool Predict(string tokenType);
	void ifMatch(string tokenType);

	//////////////////////////////////////////
	void whichAppend();

	void SchemeAppend();

	void FactAppend();

	void RuleAppend();

	void QueryAppend();

	int schemeNum;
	int factNum;
	int ruleNum;
	int queryNum;
	int tempParamCount;
	int tempPredCount;
	string tempExpressionString;


	//////////////////////////////////////////

	void WriteToOutput();
	void WriteToConsole();

	void datalogProgram();
	void scheme();
	void schemeList();
	void fact();
	void factList();
	void rule();
	void ruleList();
	void query();
	void queryList();
	void idList();
	void stringList();
	void headPredicate();
	void predicate();
	void predicateList();
	void parameterList();
	void parameter();
	void expression();
	void oprtr();
	vector<Predicate> Get_Schemes()
	{
		return schemes;
	}
	vector<Predicate> Get_Facts()
	{
		return facts;
	}
	vector<Rule> Get_Rules()
	{
		return rules;
	}
	vector<Predicate> Get_Queries()
	{
		return queries;
	}
	vector<string> Get_Domain()
	{
		return domain;
	}
	bool isItValid()
	{
		return isValid;
	}
};