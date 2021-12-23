//#ifndef RELATION_H
//#define RELATION_H

#pragma once

#include <set>
#include <sstream>
#include <iostream>
#include <map>
#include "Tuple.h"
#include "Scheme.h"
#include "Predicate.h"

using namespace std;

class Relation
{
private:
	string name; //Name of Scheme
	Scheme scheme; //Scheme is one vector of attributes
	set<Tuple> tuples; //These are Facts
	
public:
	Relation();
	~Relation();
	struct miniRelation //This is a column in a relation when you want it
	{
		string name;
		Tuple tuple;
	};
	vector<string> lexParams(string thisInput);
	void Set_Name(string aName);
	void Set_Attributes(Scheme & aScheme);
	void Set_Facts(set<Tuple> & aTupleSet);

	Scheme scheme_Lexer(string strS);

	string getName()
	{
		return name;
	}

	const string toString();

	void Select(string & strS);

	void Project(string & strS);

	void Rename(string & strS);

	set<Tuple> Union(Relation & thisR);

	void Join(Relation & thisR);

	set<Tuple> & Get_Facts();

	Scheme & Get_Scheme()
	{
		return this->scheme;
	}

	vector<string> Vector_Attributes();

	bool is_Constant(string str5);

	Scheme vectorToScheme(vector<string> & result);

	void addNewFact(Tuple & newFact);

	set<Tuple> & Get_Fact_Reference()
	{
		return tuples;
	}

	void Remove_Tuples()
	{
		set<Tuple> emptyT;
		Set_Facts(emptyT);
	}

	void Evaluate_Single_Query(bool isRules, string & queryString, Predicate & thisQuery);

	string ConvVectorString(vector<string> & theseParams);
};

//#endif



