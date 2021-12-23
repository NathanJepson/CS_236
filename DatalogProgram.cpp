#include "DatalogProgram.h"
#include "Tokens.h"
#include <iostream>
#include <exception>
#include <algorithm>
#include <set>

using namespace std;

DatalogProgram::DatalogProgram(vector<string> tokens)
{
	ofstream output;
	input = tokens;

	isValid = false;

	offendingToken = "";
	isSchemes = false;
	isFacts = false;
	isRules = false;
	isHead = false;
	isBody = false;
	isQueries = false;
	inExpression = false;
	recentWhatIs = "";

	schemeNum = 0;
	factNum = 0;
	ruleNum = 0;
	queryNum = 0;
	tempParamCount = 0;
	tempPredCount = 0;

	for (unsigned i = 0; i < input.size(); i++)
	{

		string temp = input.at(i);
		string original = temp;

		int whereComma = temp.find(",");
		original = original.substr(whereComma + 1, temp.size());
		temp = temp.substr(1, whereComma - 1);

		if (temp == "COMMA")
		{
			unsigned y = original.size();
			original = original.substr(3, y);
			string c = string(1, original.at(1));
			valueLineNum temporary = { ",", c };
			endTokens.push_back(temporary);
			tokenTypes.push_back("COMMA");
		}
		else
		{
			whereComma = original.find(",");
			char t = original.at(whereComma + 1);
			string c = string(1, t);
			original = original.substr(1, whereComma - 2);
			valueLineNum temporary = { original, c };
			endTokens.push_back(temporary);
			tokenTypes.push_back(temp);
		}
	}
}

DatalogProgram::~DatalogProgram() {}

bool DatalogProgram::Match(string tokenType)
{
	offendingToken = input.at(0);
	string temp = tokenTypes.at(0);
	recentWhatIs = endTokens.at(0).value;

	while (temp == "COMMENT")
	{
		input.erase(input.begin());
		tokenTypes.erase(tokenTypes.begin());
		endTokens.erase(endTokens.begin());

		offendingToken = input.at(0);
		temp = tokenTypes.at(0);
		recentWhatIs = endTokens.at(0).value;
	}

	if (temp.compare(tokenType) == 0)
	{
		input.erase(input.begin());
		tokenTypes.erase(tokenTypes.begin());
		endTokens.erase(endTokens.begin());
		return true;
	}
	else
	{
		return false;
	}
}

bool DatalogProgram::Predict(string tokenType)
{
	string temp = tokenTypes.at(0);
	offendingToken = input.at(0);
	recentWhatIs = endTokens.at(0).value;

	while (temp == "COMMENT")
	{
		input.erase(input.begin());
		tokenTypes.erase(tokenTypes.begin());
		endTokens.erase(endTokens.begin());

		offendingToken = input.at(0);
		temp = tokenTypes.at(0);
		recentWhatIs = endTokens.at(0).value;
	}

	if (temp.compare(tokenType) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DatalogProgram::ifMatch(string tokenType)
{
	if (Match(tokenType) == true)
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::SchemeAppend()
{
	vector<Parameter> emptyV;
	Parameter newParam = Parameter(recentWhatIs);
	if (tempParamCount == 0)
	{
		emptyV.push_back(newParam);
		schemes.at(schemeNum).Get_Params() = emptyV;
	}
	else
	{
		schemes.at(schemeNum).Get_Params().push_back(newParam);
	}
	tempParamCount++;
}

void DatalogProgram::FactAppend()
{
	vector<Parameter> emptyV;
	Parameter newParam = Parameter(recentWhatIs);
	if (tempParamCount == 0)
	{
		emptyV.push_back(newParam);
		facts.at(factNum).Get_Params() = emptyV;
	}
	else
	{
		facts.at(factNum).Get_Params().push_back(newParam);
	}
	tempParamCount++;
}

void DatalogProgram::RuleAppend()
{
	vector<Parameter> emptyV;
	vector<Predicate> emptyPred;
	Parameter newParam = Parameter(recentWhatIs);
	if (!isBody)
	{
		if (tempParamCount == 0)
		{
			emptyV.push_back(newParam);
			rules.at(ruleNum).Get_Head().Get_Params() = emptyV;
		}
		else
		{
			rules.at(ruleNum).Get_Head().Get_Params().push_back(newParam);
		}
	}
	else
	{
		if (tempPredCount == 0)
		{

			if (tempParamCount == 0)
			{
				emptyV.push_back(newParam);
				Predicate tempPred;
				tempPred.Get_Params() = emptyV;
				tempPred.Get_ID() = rules.at(ruleNum).Get_Body().at(0).Get_ID(); //This solves the ID of predicate error of first predicate in the rule's body
				emptyPred.push_back(tempPred);
				rules.at(ruleNum).Get_Body() = emptyPred;
			}
			else
			{
				rules.at(ruleNum).Get_Body().at(tempPredCount).Get_Params().push_back(newParam);
			}
		}
		else
		{
			if (tempParamCount == 0)
			{
				emptyV.push_back(newParam);
				rules.at(ruleNum).Get_Body().at(tempPredCount).Get_Params() = emptyV;
			}
			else
			{
				rules.at(ruleNum).Get_Body().at(tempPredCount).Get_Params().push_back(newParam);
			}
		}
	}
	tempParamCount++;
}

void DatalogProgram::QueryAppend()
{
	vector<Parameter> emptyV;
	Parameter newParam = Parameter(recentWhatIs);
	if (tempParamCount == 0)
	{
		emptyV.push_back(newParam);
		queries.at(queryNum).Get_Params() = emptyV;
	}
	else
	{
		queries.at(queryNum).Get_Params().push_back(newParam);
	}
	tempParamCount++;
}

void DatalogProgram::whichAppend()
{
	if (!isQueries)
	{
		if (!isRules)
		{
			if (!isFacts)
			{
				SchemeAppend();
			}
			else
			{
				FactAppend();
			}
		}
		else
		{
			RuleAppend();
		}
	}
	else
	{
		QueryAppend();
	}
}

void DatalogProgram::WriteToConsole()
{
	cout << output.str();
}

void DatalogProgram::WriteToOutput()
{
	
	output << "Success!" << endl
		<< "Schemes(" << schemeNum << "):" << endl;
	for (int i = 0; i < schemeNum; i++)
	{
		output << "  " << schemes.at(i).to_string() << endl;
	}
	output << "Facts(" << factNum << "):" << endl;
	for (int i = 0; i < factNum; i++)
	{
		output << "  " << facts.at(i).to_string() << "." << endl;
		for (unsigned j = 0; j < facts.at(i).Get_Params().size(); j++)
		{
			domain.push_back(facts.at(i).Get_Params().at(j).getValue());
		}

	}
	output << "Rules(" << ruleNum << "):" << endl;
	for (int i = 0; i < ruleNum; i++)
	{
		output << "  " << rules.at(i).to_string() << "." << endl;
	}
	output << "Queries(" << queryNum << "):" << endl;
	for (int i = 0; i < queryNum; i++)
	{
		output << "  " << queries.at(i).to_string() << "?" << endl;
	}

	vector<string> sortedParameters = domain;
	sort(sortedParameters.begin(), sortedParameters.end());
	set<string> s;
	unsigned size = sortedParameters.size();
	for (unsigned i = 0; i < size; i++)
	{
		s.insert(sortedParameters[i]);
	}
	sortedParameters.assign(s.begin(), s.end());
	domain = sortedParameters;

	output << "Domain(" << domain.size() << "):" << endl;
	for (unsigned i = 0; i < domain.size(); i++)
	{
		output << "  " << domain.at(i) << endl;
	}

}

//______________________________________________________________________________________________________________________________________________

void DatalogProgram::Parse()
{
	try
	{
		datalogProgram();
	}
	catch (invalid_argument &e)
	{
		output << "Failure!" << endl
			<< "  " << offendingToken;
		return;
	}

	return;
}

void DatalogProgram::datalogProgram()
{
	ifMatch("SCHEMES");
	ifMatch("COLON");
	scheme();
	schemeList();

	ifMatch("FACTS");
	ifMatch("COLON");
	isFacts = true;
	factList();
	//isFacts = false;

	ifMatch("RULES");
	ifMatch("COLON");
	isRules = true;
	ruleList();

	ifMatch("QUERIES");
	ifMatch("COLON");
	isQueries = true;
	query();
	queryList();

	isValid = true;
	WriteToOutput();
	return;
}

void DatalogProgram::scheme()
{
	Predicate empty;
	vector<Parameter> emptyV;

	ifMatch("ID");
	schemes.push_back(empty);
	schemes.at(schemeNum).Get_ID() = recentWhatIs;
	//schemeNum++; //FIXME

	ifMatch("LEFT_PAREN");

	ifMatch("ID");
	SchemeAppend();

	if (Predict("COMMA"))
	{
		idList();
	}
	ifMatch("RIGHT_PAREN");

	tempParamCount = 0;
	schemeNum++;
	return;
}

void DatalogProgram::schemeList()
{
	if (Predict("ID"))
	{
		scheme();
		schemeList();
	}
	else if (Predict("FACTS"))
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::fact()
{
	Predicate empty;
	vector<Parameter> emptyV;

	ifMatch("ID");
	facts.push_back(empty);
	facts.at(factNum).Get_ID() = recentWhatIs;
	//factNum++; //FIXME

	ifMatch("LEFT_PAREN");

	ifMatch("STRING");
	FactAppend();

	if (Predict("COMMA")) //This was placed here when error was caught in Project 3
	{
		stringList();
	}
	ifMatch("RIGHT_PAREN");

	ifMatch("PERIOD");

	tempParamCount = 0;
	factNum++;
	return;
}

void DatalogProgram::factList()
{
	if (Predict("ID"))
	{
		fact();
		factList();
	}
	else if (Predict("RULES"))
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::rule()
{
	isHead = true;
	headPredicate();
	ifMatch("COLON_DASH");
	//goodString += " :- ";
	isHead = false;
	isBody = true;
	tempPredCount = 0;
	predicate();
	predicateList();
	ifMatch("PERIOD");

	ruleNum++;
	isBody = false;
	tempPredCount = 0;
	tempParamCount = 0;
	return;
}

void DatalogProgram::ruleList()
{
	if (Predict("ID"))
	{
		rule();
		ruleList();
	}
	else if (Predict("QUERIES"))
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::query()
{
	predicate();
	ifMatch("Q_MARK");

	queryNum++;
	tempParamCount = 0;
	return;
}

void DatalogProgram::queryList()
{
	if (Predict("EOF"))
	{
		return;
	}
	else if (Predict("ID"))
	{
		query();
		queryList();
	}
	else
	{
		ifMatch("EOF"); //THIS WILL THROW THE EXCEPTION IN ANOTHER FUNCTION
	}
}

void DatalogProgram::idList()
{
	ifMatch("COMMA");

	ifMatch("ID");
	whichAppend();

	if (Predict("COMMA"))
	{
		idList();
	}
	else if (Predict("RIGHT_PAREN"))
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::stringList()
{
	ifMatch("COMMA");

	ifMatch("STRING");
	whichAppend();

	if (Predict("COMMA"))
	{
		stringList();
	}
	else if (Predict("RIGHT_PAREN"))
	{
		return;
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::headPredicate()
{
	Rule empty;
	vector<Predicate> emptyV;
	Predicate temporaryPred;

	ifMatch("ID");
	rules.push_back(empty);
	rules.at(ruleNum).Get_Head().Get_ID() = recentWhatIs;

	ifMatch("LEFT_PAREN");

	ifMatch("ID");
	RuleAppend();

	if (Predict("COMMA"))
	{
		idList();
	}
	ifMatch("RIGHT_PAREN");

	tempParamCount = 0;
	return;
}

void DatalogProgram::predicate()
{

	ifMatch("ID");
	if (isBody)
	{
		Predicate empty;
		empty.Get_ID() = recentWhatIs;
		rules.at(ruleNum).Get_Body().push_back(empty);
	}
	else if (isQueries)
	{
		Predicate empty;
		empty.Get_ID() = recentWhatIs;
		queries.push_back(empty);
	}
	else
	{
		whichAppend();
	}

	ifMatch("LEFT_PAREN");

	parameter();
	parameterList();
	ifMatch("RIGHT_PAREN");

	if (isBody && (!isQueries))
	{
		tempPredCount++;
	}

	tempParamCount = 0;
	return;
}

void DatalogProgram::predicateList()
{
	if (Predict("PERIOD"))
	{
		return;
	}
	else if (Predict("COMMA"))
	{
		ifMatch("COMMA");

		predicate();
		predicateList();
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
	return;
}

void DatalogProgram::parameterList()
{
	if (Predict("RIGHT_PAREN"))
	{
		return;
	}
	else if (Predict("COMMA"))
	{
		ifMatch("COMMA");

		parameter();
		parameterList();
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
}

void DatalogProgram::parameter()
{

	if (Predict("STRING"))
	{
		ifMatch("STRING");
		if (!inExpression)
		{
			whichAppend();
		}
		else
		{
			tempExpressionString += recentWhatIs;
		}
	}
	else if (Predict("ID"))
	{
		ifMatch("ID");
		if (!inExpression)
		{
			whichAppend();
		}
		else
		{
			tempExpressionString += recentWhatIs;
		}
	}
	else if (Predict("LEFT_PAREN"))
	{
		expression();
	}
	else
	{
		throw invalid_argument(offendingToken);
	}

	return;
}

void DatalogProgram::expression()
{
	inExpression = true;

	ifMatch("LEFT_PAREN");
	tempExpressionString += "(";
	parameter();
	oprtr();
	parameter();

	ifMatch("RIGHT_PAREN");
	tempExpressionString += ")";

	recentWhatIs = tempExpressionString;
	RuleAppend();

	inExpression = false;
	tempExpressionString = "";

	return;
}

void DatalogProgram::oprtr()
{
	if (Predict("ADD"))
	{
		ifMatch("ADD");
		tempExpressionString += "+";
	}
	else if (Predict("MULTIPLY"))
	{
		ifMatch("MULTIPLY");
		tempExpressionString += "*";
	}
	else
	{
		throw invalid_argument(offendingToken);
	}
	return;
}