#include "Relation.h"

using namespace std;

Relation::Relation() {}
Relation::~Relation() {}


vector<string> Relation::lexParams(string thisInput)
{
	vector<string> result;
	string writeParam = "";
	for (unsigned i = 0; i < thisInput.size(); i++)
	{
		//writeParam = "";
		if (i != thisInput.size() - 1)
		{
			if (thisInput.at(i) != ',')
			{
				writeParam += thisInput.substr(i, 1);
				continue;
			}
			else
			{
				result.push_back(writeParam);
				writeParam = "";
				continue;
			}
		}
		else
		{
			writeParam += thisInput.substr(i, 1);
			result.push_back(writeParam);
		}

	}
	return result;
}

void Relation::Set_Name(string aName)
{
	name = aName;
}

void Relation::Set_Attributes(Scheme & aScheme)
{
	scheme = aScheme;
}

void Relation::Set_Facts(set<Tuple> & aTupleSet)
{
	tuples = aTupleSet;
}

const string Relation::toString()
{
	ostringstream oss;
	set<Tuple>::iterator it12;
	for (it12 = tuples.begin(); it12 != tuples.end(); it12++)
	{
		for (unsigned i = 0; i < scheme.size(); i++)
		{
			if (i == 0)
			{
				oss << "  ";
			}
			Tuple thisT = *it12;
			oss << scheme.at(i) << "=" << thisT.at(i);
			if (i != scheme.size() - 1)
			{
				oss << ", ";
			}
			else
			{
				oss << endl;
			}
		}
	}
	return oss.str();
}

void Relation::Select(string & strS)
{
	vector<string> theseArguments = lexParams(strS);
	
	for (unsigned i = 0; i < theseArguments.size(); i++)
	{
		set<Tuple> newTuples; //Put inside the for loop just in case an argument is not found
		bool isConstant = false;
		string str5 = theseArguments.at(i);
		isConstant = is_Constant(str5);

		if (isConstant)
		{
			set<Tuple> someFacts = this->Get_Facts();
			set<Tuple>::iterator it8;
			for (it8 = someFacts.begin(); it8 != someFacts.end(); it8++)
			{
				Tuple thisTuple = *it8;
				if (thisTuple.at(i) == theseArguments.at(i))
				{
					newTuples.insert(thisTuple);
				}
			}
			this->Set_Facts(newTuples);
		}
		else
		{
			bool overarchingIf = false;
			set<Tuple> someFacts = this->Get_Facts();
			set<Tuple>::iterator it9;
			if (i != theseArguments.size() - 1)
			{
				for (unsigned j = (i + 1); j < theseArguments.size(); j++)
				{

					if ((theseArguments.at(i) == theseArguments.at(j)) &&
						(is_Constant(theseArguments.at(j)) == false) &&
						(is_Constant(theseArguments.at(i)) == false))
					{
						overarchingIf = true;
						for (it9 = someFacts.begin(); it9 != someFacts.end(); it9++)
						{
							Tuple thisTuple = *it9;
							if (thisTuple.at(i) == thisTuple.at(j))
							{
								newTuples.insert(thisTuple);
							}
						}
					}
				}
			}
			if (!overarchingIf)
			{
				this->Set_Facts(someFacts);
				continue;
			}
			this->Set_Facts(newTuples);
		}
	}
}

void Relation::Project(string & strS)
{

	Scheme newScheme = scheme_Lexer(strS);
	set<Tuple>::iterator jeff;
	set<Tuple> fun;

	for (jeff = this->tuples.begin(); jeff != this->tuples.end(); jeff++)
	{
		Tuple goodOne;
		for (unsigned k = 0; k < newScheme.size(); k++)
		{
			for (unsigned i = 0; i < this->scheme.size(); i++)
			{
				if (this->scheme.at(i) == newScheme.at(k))
				{
					goodOne.push_back((*jeff).at(i));
				}
			}
		}
		fun.insert(goodOne);
	}

	this->Set_Facts(fun);
	this->Set_Attributes(newScheme);
	return;
}

void Relation::Rename(string & strS)
{
	vector<string> theseArguments = lexParams(strS);
	
	Scheme tempS;
	for (unsigned i = 0; i < theseArguments.size(); i++)
	{
		tempS.push_back(theseArguments.at(i));
	}
	this->Set_Attributes(tempS);
}

set<Tuple> Relation::Union(Relation & thisR)
{
	//Relation relation2 = thisR; //passed-in one
	//Relation relation1; //will set attributes of this relation
	bool isSimilar = false;

	try
	{
		if (thisR.Vector_Attributes().size() != this->Vector_Attributes().size())
		{
			throw invalid_argument("Oh no!");
		}
		vector<string> temp1 = thisR.Vector_Attributes();
		vector<string> temp2 = this->Vector_Attributes();

		for (unsigned i = 0; i < temp1.size(); i++)
		{
			isSimilar = false;
			set<string> secondOne;
			for (unsigned j = 0; j < temp2.size(); j++)
			{
				secondOne.insert(temp2.at(j));
			}
			set<string>::iterator it = secondOne.begin();
			for (it = secondOne.begin(); it != secondOne.end(); it++)
			{
				if (*it == temp1.at(i))
				{
					isSimilar = true;
				}
			}
			if (!isSimilar)
			{
				throw invalid_argument("Attribute not found.");
			}
		}
	}
	catch (invalid_argument & e)
	{
		vector<Tuple> emptyT;
		cout << endl << "Invalid union. Try using relations with the same attributes." << endl;
		//relation1.Set_Name("Invalid40957");
		this->Set_Name("Invalid40957");
		return this->Get_Facts();
	}

	set<Tuple> result;
	
	set<Tuple>::iterator callie;

	for (callie = thisR.Get_Facts().begin(); callie != thisR.Get_Facts().end(); callie++)
	{
		if (this->Get_Facts().find(*callie) == this->Get_Facts().end())
		{
			result.insert(*callie);
		}
		this->Get_Facts().insert(*callie);
	}

	return result;
}

void Relation::Join(Relation & thisR) 
{
	Relation empty;
	Scheme newAttributes;

	vector<string> columns1;
	map<string,int> lookupCol1;

	vector<string> columns2;
	vector<string> uniqueSecond;
	
	map<string, int> lookupCol2;

	vector<string> sameColumns;
	
	
	for (unsigned i = 0; i < this->Vector_Attributes().size(); i++)
	{
		columns1.push_back(this->Vector_Attributes().at(i));
		lookupCol1.insert(pair<string,int>(this->Vector_Attributes().at(i),i));
	}

	for (unsigned i = 0; i < thisR.Vector_Attributes().size(); i++)
	{
		if (lookupCol1.find(thisR.Vector_Attributes().at(i)) != lookupCol1.end())
		{
			sameColumns.push_back(thisR.Vector_Attributes().at(i));
		}
		else
		{
			uniqueSecond.push_back(thisR.Vector_Attributes().at(i));
		}

		columns2.push_back(thisR.Vector_Attributes().at(i));
		lookupCol2.insert(pair<string, int>(thisR.Vector_Attributes().at(i), i));
	}

	for (unsigned i = 0; i < columns1.size(); i++)
	{
		newAttributes.push_back(columns1.at(i));
	}
	for (unsigned i = 0; i < uniqueSecond.size(); i++)
	{
		newAttributes.push_back(uniqueSecond.at(i));
	}

	this->Set_Attributes(newAttributes);

	set<Tuple> factList1 = this->Get_Facts(); //CurrentRelation
	set<Tuple> factList2 = thisR.Get_Facts(); //New facts

	set<Tuple> resultList; //Joined tuples

	set<Tuple>::iterator shonda;
	set<Tuple>::iterator landa;

	if (newAttributes == thisR.Get_Scheme() && newAttributes == this->Get_Scheme()) //Columns are identical
	{
		for (shonda = thisR.Get_Facts().begin(); shonda != thisR.Get_Facts().end(); shonda++)
		{
			this->Get_Fact_Reference().insert(*shonda);
		}
		return;
	}
	else //Columns are not identical
	{
		Tuple tuple88;
		Tuple tuple89;
		if (sameColumns.size() != 0) //This means columns are similar
		{
			for (shonda = factList1.begin(); shonda != factList1.end(); shonda++) //For every pair of Tuples; can they join?
			{
				for (landa = factList2.begin(); landa != factList2.end(); landa++)
				{
					tuple88 = *shonda;
					tuple89 = *landa;
					for (unsigned k = 0; k < columns1.size(); k++) //For each attribute name in 1st table
					{
						if (lookupCol2.find(columns1.at(k)) != lookupCol2.end()) //Is in both
						{
							if (tuple88.at(k) != tuple89.at(lookupCol2.find(columns1.at(k))->second))
							{
								break;
							}
						}
						if (k == columns1.size() - 1)
						{
							for (unsigned i = 0; i < uniqueSecond.size(); i++)
							{
								tuple88.push_back(tuple89.at(lookupCol2.find(uniqueSecond.at(i))->second));
							}
							resultList.insert(tuple88);
						}
					}
				}
			}
		}
		

		else //No similar columns
		{
			for (shonda = factList1.begin(); shonda != factList1.end(); shonda++)
			{
				for (landa = factList2.begin(); landa != factList2.end(); landa++)
				{
					tuple88 = *shonda;
					tuple89 = *landa;

					for (unsigned p = 0; p < tuple89.size(); p++)
					{
						tuple88.push_back(tuple89.at(p));
					}
					resultList.insert(tuple88);
				}
			}
		}
		this->Set_Facts(resultList);
	}
}

set<Tuple> & Relation::Get_Facts()
{
	return tuples;
}

vector<string> Relation::Vector_Attributes()
{
	vector<string> result;
	for (unsigned i = 0; i < scheme.size(); i++)
	{
		result.push_back(scheme.at(i));
	}
	return result;
}

bool Relation::is_Constant(string str5)
{
	bool isConstant = false;
	char c = str5.back();

	if (str5.substr(0, 1) == "\'" && c == '\'')
	{
		isConstant = true;
	}

	return isConstant;
}

Scheme Relation::vectorToScheme(vector<string> & result)
{
	Scheme resultS;
	for (unsigned i = 0; i < result.size(); i++)
	{
		resultS.push_back(result.at(i));
	}
	return resultS;
}

void Relation::addNewFact(Tuple & newFact)
{
	(this->tuples).insert(newFact);
}

Scheme Relation::scheme_Lexer(string strS)
{
	Scheme result;
	string writeParam = "";
	for (unsigned i = 0; i < strS.size(); i++)
	{
		//writeParam = "";
		if (i != strS.size() - 1)
		{
			if (strS.at(i) != ',')
			{
				writeParam += strS.substr(i, 1);
				continue;
			}
			else
			{
				result.push_back(writeParam);
				writeParam = "";
				continue;
			}
		}
		else
		{
			writeParam += strS.substr(i, 1);
			result.push_back(writeParam);
		}

	}
	return result;
}

void Relation::Evaluate_Single_Query(bool isRules, string & queryString, Predicate & thisQuery)
{
	this->Select(queryString); //Select

	if (isRules)
	{
		this->Rename(queryString); //This finally gives the new columns their new names
	}
	else
	{
		vector<Parameter> thisQueryParams = thisQuery.Get_Params();
		vector<string> remaining;
		vector<string> other;
		for (unsigned b = 0; b < thisQueryParams.size(); b++) //////Now take out constant columns
		{
			if (thisQueryParams.at(b).Is_Constant() == false)
			{
				remaining.push_back(this->Vector_Attributes().at(b));
				/*This will get true relation colum names to make it easier to Project later*/
				other.push_back(thisQuery.Get_Params().at(b).getValue());
				//This will get new relation column names from the query. Will be useful for Rename
			}
		}
		string newParamString = ConvVectorString(remaining);
		this->Project(newParamString); //Project
		this->Rename(ConvVectorString(other)); //This finally gives the new columns their new names
	}
	//Project 5; above Function takes too long
}

string Relation::ConvVectorString(vector<string> & theseParams) 
{
	//vector<string> theseParams = Get_Vector_Params();
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