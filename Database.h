#pragma once
#include <map>
#include "Relation.h"

using namespace std;

//Key: Table or Relation name; value is the relation/table
class Database : public map<string, Relation>
{
private:
	
public:
	Database();
	~Database();

	Relation & Get_Relation(string relationName)
	{
		return(*this)[relationName];
	}

	Relation & Get_Relation_Reference(string relationName)
	{
		return(*this)[relationName];
	}
	Relation Get_Relation_Copy(string relationName)
	{
		return(*this)[relationName];
	}

	const int tupleNums()
	{
		int result = 0;
		map<string, Relation>::iterator it13;
		for (it13 = this->begin(); it13 != this->end(); it13++)
		{
			result += it13->second.Get_Facts().size();
		}

		return result;
	}	
};

