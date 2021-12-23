#include "Interpreter.h"


using namespace std;

Interpreter::Interpreter()
{
	iteratorT = 0;
	SCCtracker3 = -1;
	ssIterator = 0;
	Debugger = 0;
}

Interpreter::~Interpreter()
{
}

Database & Interpreter::Get_DB(int & where5)
{
	return myDatabase;
}

void Interpreter::Acquire_DatalogProgram(vector<Predicate> schemesQ, vector<Predicate> factsQ,
	vector<Rule> rulesQ, vector<Predicate> queriesQ, vector<string> domainQ)
{
	dataProgram thisProgram;
	programsQ.push_back(thisProgram);
	programsQ[iteratorT].schemesS = schemesQ;
	programsQ[iteratorT].factsS = factsQ;
	programsQ[iteratorT].rulesS = rulesQ;
	programsQ[iteratorT].queriesS = queriesQ;
	programsQ[iteratorT].domainS = domainQ;
	Write_Database();
	iteratorT++;
}

void Interpreter::Write_Database()
{
	vector<Predicate> schemes = programsQ.at(iteratorT).schemesS;
	vector<Predicate> facts = programsQ.at(iteratorT).factsS;
	vector<Rule> rules = programsQ.at(iteratorT).rulesS;
	vector<Predicate> queries = programsQ.at(iteratorT).queriesS;

	Database newDB;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	map<string, Scheme> schemeAttributes;

	//This will assign a new table and map each scheme and their attributes in a temporary map
	for (unsigned i = 0; i < schemes.size(); i++)
	{
		Relation empty;
		string schemeName = schemes.at(i).Get_ID();
		newDB.insert(pair<string, Relation>(schemeName, empty));
		Scheme attributes; //Not to be confused with map of above

		for (unsigned j = 0; j < schemes.at(i).Get_Params().size(); j++)
		{
			attributes.push_back(schemes.at(i).Get_Params().at(j).getValue());
		}

		schemeAttributes.insert(pair<string, Scheme>(schemeName, attributes));
	}

	map<string, set<Tuple>> factTuplePairs;

	//This will append to a temporary map all facts with Key: schemeName (named as factName) and Value: set of matching tuples
	for (unsigned i = 0; i < facts.size(); i++)
	{
		string factName = facts.at(i).Get_ID();
		Tuple T;
		for (unsigned j = 0; j < facts.at(i).Get_Params().size(); j++)
		{
			T.push_back(facts.at(i).Get_Params().at(j).getValue());
		}

		if (factTuplePairs.find(factName) == factTuplePairs.end()) {
			//not found
			set<Tuple> factTuple;
			factTuple.insert(T);
			factTuplePairs.insert(pair<string, set<Tuple>>(factName, factTuple));
		}
		else {
			//found
			map<string, set<Tuple>>::iterator it11;
			for (it11 = factTuplePairs.begin(); it11 != factTuplePairs.end(); it11++)
			{
				if (it11->first == factName)
				{
					it11->second.insert(T);
				}
			}
		}
	}


	map<string, Relation>::iterator it1;

	//This creates a table and puts it in the database
	for (it1 = newDB.begin(); it1 != newDB.end(); it1++)
	{
		string relationName = it1->first;
		Relation table;

		map<string, Scheme>::iterator it2;
		for (it2 = schemeAttributes.begin(); it2 != schemeAttributes.end(); it2++)
		{
			if (it2->first == relationName)
			{
				table.Set_Name(it2->first);
				table.Set_Attributes(it2->second);
			}
		}

		map<string, set<Tuple>>::iterator it3;
		for (it3 = factTuplePairs.begin(); it3 != factTuplePairs.end(); it3++)
		{
			if (it3->first == relationName)
			{
				table.Set_Facts(it3->second);
			}
		}


		//This will set the already empty table in the database to the created table
		map<string, Relation>::iterator it4;
		for (it4 = newDB.begin(); it4 != newDB.end(); it4++)
		{
			if (it4->first == relationName)
			{
				it4->second = table;
			}
		}
	}

	//This will store the database in the relevant vector
	//databases.push_back(newDB);
	myDatabase = newDB;
}

void Interpreter::Evaluate_Queries(int location)
{
	//Project 5, rules already Evaluated
	//The location designates which data program we are talking about

	//Database & DBQ = myDatabase;
	vector<Predicate> allQueries = programsQ.at(location).queriesS; //programsQ is only vector of data programs 

	//vector<Rule> allRules = programsQ.at(location).rulesS; //Commented Project 5
	//Fixed_Point_Algorithm(programsQ.at(location), DBQ); //Commented Project 5

	output << endl << "Query Evaluation" << endl;
	for (unsigned i = 0; i < allQueries.size(); i++)
	{
		Predicate thisQuery = allQueries.at(i);
		string relationName = thisQuery.Get_ID();


		Relation goodResult = myDatabase.Get_Relation(relationName);

		//////////////////Now get new relation with SELECT
		string queryString = thisQuery.String_Params();
		goodResult.Evaluate_Single_Query(false, queryString, thisQuery);

		if (goodResult.Get_Facts().size() == 0)
		{
			output << allQueries.at(i).to_string() << "? No" << endl;
		}
		else
		{
			output << allQueries.at(i).to_string() << "? Yes(" << goodResult.Get_Facts().size() << ")" << endl;
			output << goodResult.toString();
		}
	}

	WriteToConsole();
}

string Interpreter::convVectorString(vector<string> & theseParams)
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

string Interpreter::String_Params_2(vector<string> & thisVector)
{
	string result = "";
	for (unsigned i = 0; i < thisVector.size(); i++)
	{
		result += thisVector.at(i);
		if (i != thisVector.size() - 1)
		{
			result += ",";
		}
	}
	return result;
}

void Interpreter::Evaluate_Graph(int location)
{
	//Database & DBQ = databases.at(location);
	Database original = myDatabase;
	vector<Rule> allRules = programsQ.at(location).rulesS; //programsQ is only vector of data programs

	if (allRules.size() == 0)
	{
		output << "Dependency Graph" << endl << endl << "Rule Evaluation" << endl;
		return;
	}
	map<int, Node> nodes;
	map<int, Node> shadowNodes;
	
	for (unsigned i=0; i < allRules.size(); i++)
	{
		string ruleHeadName = allRules.at(i).Get_Head().Get_ID();
		set<int> dependencies;

		for (unsigned j = 0; j < allRules.at(i).Get_Body().size(); j++)
		{
			for (unsigned k = 0; k < allRules.size(); k++)
			{
				if (allRules.at(i).Get_Body().at(j).Get_ID() == allRules.at(k).Get_Head().Get_ID())
				{
					dependencies.insert(k);
				}
			}
		}
		Node NodeN = Node(dependencies);
		nodes.insert(pair<int, Node>(i, NodeN));
		shadowNodes.insert(pair<int, Node>(i, NodeN));
	}

	output << "Dependency Graph" << endl;
	map<int, Node>::iterator it;
	set<int>::iterator it2;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		output << "R" << it->first << ":" << (it->second).To_String() << endl;
	}
	output << endl;
	//WriteToConsole();

	//Reverse dependency Graph
	map<int, Node> reverseDep;
	set<int> emptyS;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		Node emptyN = Node(emptyS);
		reverseDep.insert(pair<int, Node>(it->first, emptyN));
	}
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		int thisNode = it->first;
		for (it2 = (it->second).getPredecessors().begin(); it2 != (it->second).getPredecessors().end(); it2++)
		{
			(reverseDep.find(*it2)->second).getPredecessors().insert(it->first);
			//(nodes.find(*it2)->second.getChildren()).insert(it->first); ///Not true but used below
			(shadowNodes.find(*it2)->second.getChildren()).insert(it->first);

			nodes.find(thisNode)->second.getChildren().insert(*it2); //Gets children
		}
	}

	map<int, Node*> reverseDepPoint;
	map<int, Node*>::iterator it44;


	//output << "Reverse Dependency Graph" << endl;
	
	for (it = reverseDep.begin(); it != reverseDep.end(); it++)
	{
		reverseDepPoint.insert(pair<int,Node*>(it->first, &it->second));
		//output << "R" << it->first << ":" << it->second.To_String() << endl;
	}
	//output << endl;

	for (it44 = reverseDepPoint.begin(); it44 != reverseDepPoint.end(); it44++)
	{
		Node* thisNode = it44->second;
		for (it2 = thisNode->getPredecessors().begin(); it2 != thisNode->getPredecessors().end(); it2++)
		{
			int thisThing = *it2;
			int otherThing = it44->first;
			reverseDepPoint.find(otherThing)->second->getChildren().insert(thisThing);
		}
	}

	//WriteToConsole();

	//Getting Post Order Numbers
	stack<int> postOrderHelp;
	for (unsigned i = 1; i < reverseDepPoint.size() + 1; i++)
	{
		postOrderHelp.push(i);
	}
	reverseDepPoint = Post_Order_Helper(reverseDepPoint, reverseDepPoint.begin()->first, postOrderHelp);

	//////////////////////////////////////////////////////////////////////////////////////////

	//output << "Post Order Numbers" << endl;
	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		int tempT = it->first;
		int tempTT = reverseDepPoint.find(tempT)->second->Get_PostOrder();
		it->second.Set_PostOrder(tempTT);
	}

	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		Node thisNode = it->second;
		//output << it->first << ":  " << thisNode.Get_PostOrder() << endl;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////


	int firstNode = -1;
	vector<Node> theNodes1;
	vector<Node> betaNodes1;
	for (unsigned i = 0; i < allRules.size(); i++)
	{
		int tempTT = i;
		nodes.find(i)->second.Set_Name(tempTT);
		theNodes1.push_back(nodes.find(i)->second);
	}
	for (unsigned i = 0; i < allRules.size(); i++)
	{
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			if (it->second.Get_PostOrder() == i + 1)
			{
				if (i == 0)
				{
					firstNode = it->second.Get_Name();
				}
				betaNodes1.push_back(it->second);
			}
		}
	}
	//WriteToConsole();
	

	//Using Post-Order Numbers to find SCCs
	set<int> empty;
	vector<Node*> theNodes;
	vector<Node*> betaNodes;
	for (unsigned i = 0; i < theNodes1.size(); i++)
	{
		Node* thisNode = &theNodes1.at(i);
		theNodes.push_back(thisNode);
	}
	for (unsigned i = 0; i < betaNodes1.size(); i++)
	{
		for (unsigned j = 0; j < theNodes.size(); j++)
		{
			Node thisNode = betaNodes1.at(i);
			int thisInt = thisNode.Get_Name();
			if (theNodes.at(j)->Get_Name() == thisInt)
			{
				betaNodes.push_back(theNodes.at(j));
			}
		}
	}
	
	while (SCCtracker != traverseGraph(betaNodes, theNodes, firstNode))
	{
		if (!SCCtracker2.empty())
		{
			SCCs.push_back(SCCtracker2);
		}
		SCCtracker = SCCtracker2;
		SCCtracker2 = empty;
		firstNode = SCCtracker3;
	}
	
	//Now we use SCCs to evaluate Rules
	for (unsigned i = 0; i < theNodes.size(); i++)
	{
		theNodes.at(i)->Set_Flag(); //Set all nodes to not visited
	}
	vector<Rule> emptyRules;
	vector<vector<Node*>> SCC_Plus;

	for (unsigned i = 0; i < SCCs.size(); i++)
	{
		vector<Node*> tempV;
		for (it2 = SCCs.at(i).begin(); it2 != SCCs.at(i).end(); it2++)
		{
			tempV.push_back(theNodes.at(*it2));
		}
		SCC_Plus.push_back(tempV);
	}


	programsQ.at(location).rulesS = emptyRules;
	vector<vector<Node*>> NodeLord;

	//No need to rearrange nodes in each SCC

	for (unsigned i = 0; i < theNodes.size(); i++)
	{
		theNodes.at(i)->getChildren() = (shadowNodes.find(i))->second.getChildren();
	}
	NodeLord = SCC_Plus;

	for (unsigned i = 0; i < NodeLord.size(); i++)
	{
		map<int, loggs> emptyRR;
		ssIterator = 0;
		ss = emptyRR;

		vector<Node*> currentSCC = NodeLord.at(i);
		//cout << "Into fixed point" << endl;
		Fixed_Point_Algorithm(currentSCC, allRules, theNodes);
		//cout << "Out of fixed Point" << endl;
		final.push_back(ss);
	}
	
	
	/////////////////////Writing information to console

	//thisIsIt.at(0);
	//thisIsIt2.at(0);
	ssIterator = 0;
	map<int, loggs>::iterator itQ;
	map<int, loggs>::iterator it8;
	//set<Tuple>::iterator fun;

	//No need to remove duplicate tuples
	

	output << "Rule Evaluation" << endl; //Very hard
	int goodIterator = 0;
	final.at(0);
	

	for (unsigned i = 0; i < SCCs.size(); i++)
	{
		set<int>::iterator it;
		output << "SCC: ";
		int iteratorE = 0;
		for (it = SCCs.at(i).begin(); it != SCCs.at(i).end(); it++)
		{
			output << "R" << *it;
			
			if (iteratorE != SCCs.at(i).size()-1)
			{
				output << ",";
			}
			iteratorE++;
		}
		output << endl;

		//cout << ss.find(0)->first;


		map<int, loggs> thisGamma = final.at(i);
		map<int, loggs>::iterator hope;
		for (hope = thisGamma.begin(); hope != thisGamma.end(); hope++)
		{
			output << hope->second.itsRule.to_string() << endl;
			output << hope->second.itsRelation.toString();
		}


		output << (thisGamma.size()/(SCCs.at(i).size())) << " passes: ";
		iteratorE = 0;

		for (it = SCCs.at(i).begin(); it != SCCs.at(i).end(); it++)
		{
			output << "R" << *it;

			if (iteratorE != SCCs.at(i).size() - 1)
			{
				output << ",";
			}
			iteratorE++;
		}
		output << endl;
	}


	//WriteToConsole();
}

set<int> Interpreter::traverseGraph(vector<Node*> & precedenceList, vector<Node*> & nodeList, int toVisit)
{
	
	////////////////////////////////////////////////
	Node* currentNode = nodeList.at(toVisit);
	
	if ((currentNode)->hasBeenVisited() == true)
	{
		return SCCtracker2;
	}
	currentNode->Trigger_Flag();
	SCCtracker2.insert(currentNode->Get_Name());

	set<int>::iterator it;
	
	set<int> childrenZ;
	for (unsigned i = 0; i < nodeList.size(); i++)
	{
		if (currentNode->getChildren().find(i) != currentNode->getChildren().end())
		{
			childrenZ.insert(i);
		}
	}
	for (unsigned i = 0; i < precedenceList.size(); i++)
	{
		if (childrenZ.find(i) != childrenZ.end())
		{
			if (nodeList.at(i)->hasBeenVisited() == false)
			{
				traverseGraph(precedenceList, nodeList, i);
			}
		}
	}
	
	for (unsigned i = 0; i < precedenceList.size(); i++)
	{
		if (precedenceList.at(i)->hasBeenVisited() == false)
		{
			SCCtracker3 = precedenceList.at(i)->Get_Name();
			break;
		}
		else
		{
			SCCtracker3 = currentNode->Get_Name();
		}
	}
	return SCCtracker2;
}

unsigned Interpreter::Fixed_Point_Algorithm(vector<Node*> thisSCC, vector<Rule> ruleList, vector<Node*> allNodes)
{
	vector<Rule> goodRules;
	set<Tuple> empty;

	if (thisSCC.size() == 1)
	{
		goodRules.push_back(ruleList.at(thisSCC.at(0)->Get_Name()));
		if (thisSCC.at(0)->getChildren().find(thisSCC.at(0)->Get_Name()) != thisSCC.at(0)->getChildren().end())
		{
			Evaluate_Rules(goodRules);

			while (true)
			{
				int tracker = myDatabase.tupleNums();
				Evaluate_Rules(goodRules);

				if (tracker == myDatabase.tupleNums())
				{
					break;
				}
			}
		}
		else
		{
			Evaluate_Rules(goodRules);
		}
		return myDatabase.tupleNums();
	}
	else
	{
		for (unsigned i = 0; i < thisSCC.size(); i++)
		{
			goodRules.push_back(ruleList.at(thisSCC.at(i)->Get_Name())); //Relevant rules
		}

		Evaluate_Rules(goodRules);
		while (true)
		{
			int tracker = myDatabase.tupleNums();
			Evaluate_Rules(goodRules);

			if (tracker == myDatabase.tupleNums())
			{
				if (Debugger == 16)
				{
					//cout << __LINE__ << endl;
				}
				break;
			}
		}
		
	}
	return myDatabase.tupleNums();
}

unsigned Interpreter::Evaluate_Rules(vector<Rule> & theLaws)
{
	pastLogger = myDatabase;
	
	for (unsigned i = 0; i < theLaws.size(); i++)
	{
		Debugger++;
		//cout << "Evalutaing Rule:  " << i << "  " << Debugger << " total iterations." << endl;
		Relation resultant;
		vector<Relation> evalPredicates;

		for (unsigned j = 0; j < theLaws.at(i).Get_Body().size(); j++)
		{
			Predicate thisPredicate = theLaws.at(i).Get_Body().at(j);
			string queryString = thisPredicate.String_Params();
			resultant = myDatabase.Get_Relation_Copy(thisPredicate.Get_ID());
			resultant.Evaluate_Single_Query(true, queryString, thisPredicate); 
			//Evaluate it like query
			evalPredicates.push_back(resultant);
		}

		resultant = evalPredicates.at(0);

		if (evalPredicates.size() > 1)
		{
			for (unsigned j = 1; j < evalPredicates.size(); j++)
			{
				if (Debugger == 14)
				{
					//cout << __LINE__ << endl;
				}
				resultant.Join(evalPredicates.at(j)); //Join
			}
		}

		string queryString = theLaws.at(i).Get_Head().String_Params();
		resultant.Project(queryString);  //Project
		//////////////////////


		Relation & relevantRelationR = myDatabase.Get_Relation_Reference(theLaws.at(i).Get_Head().Get_ID());
		vector<string> newSchemeName = relevantRelationR.Vector_Attributes();
		string schemeName = String_Params_2(newSchemeName);
		resultant.Rename(schemeName); //Rename

		set<Tuple> unique = relevantRelationR.Union(resultant); //Union
		
		loggs thisTemp;
		thisTemp.name9 = relevantRelationR.getName();
		thisTemp.itsRule = theLaws.at(i);

		Relation copyR;
		copyR.Set_Name(relevantRelationR.getName());
		copyR.Set_Attributes(relevantRelationR.Get_Scheme());
		copyR.Set_Facts(unique);
		thisTemp.itsRelation = copyR;

		ss.insert(pair<int, loggs>(ssIterator, thisTemp));
		ssIterator++;
	}
	return myDatabase.tupleNums();
}

map<int, Node*> Interpreter::Post_Order_Helper(map<int, Node*> moreNodes, int targetNode, stack<int> & helpStack)
{
	map<int, Node*>::iterator it2;
	for (it2 = moreNodes.begin(); it2 != moreNodes.end(); it2++)
	{
		if (it2->second->hasBeenVisited() == false)
		{
			int temp = it2->first;
			moreNodes = Get_Post_Order(moreNodes, temp, helpStack);
			it2 = moreNodes.begin();
		}
	}
	return moreNodes;
}

map<int,Node*> Interpreter::Get_Post_Order(map<int,Node*> moreNodes, int targetNode, stack<int> & helpStack)
{
	moreNodes.find(targetNode)->second->Trigger_Flag();
	//helpStack.push(targetNode);
	Node* thisNode = moreNodes.find(targetNode)->second;
	set<int>::iterator it;
	set<int> tempT;
	for (it = thisNode->getChildren().begin(); it != thisNode->getChildren().end(); it++)
	{
		int temp = *it;
		if (moreNodes.find(temp)->second->hasBeenVisited() == false)
		{
			moreNodes = Get_Post_Order(moreNodes, temp, helpStack);
		}
	}

	moreNodes.find(targetNode)->second->Set_PostOrder(helpStack.top());
	helpStack.pop();
	return moreNodes;
}