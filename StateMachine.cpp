#include "StateMachine.h"


using namespace std;

const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

char Char_Cast(string temp)
{
	return temp.at(0);
}

string String_Cast(char tempChar)
{
	string catalyze = "";
	catalyze = "";
	catalyze += tempChar;
	catalyze = catalyze.substr(0, 1);
	return catalyze;
}

//______________________________________________________________________________________________________

StateMachine::StateMachine()
{
	lineNum = 1;
	tempLineCount = 0;
	tempWhatIs = "";
	//tokens = 0;
	debug = 0;
}

StateMachine::~StateMachine()
{
}

void StateMachine::AddLineNum()
{
	lineNum++;
}

void StateMachine::Insert(string type, string whatIs)
{
	output.str("");
	output.clear();
	string lineID = to_string(lineNum);
	output << "(" << type << ",\"" << whatIs << "\"," << lineID << ")" << endl;
	string newToken = output.str();
	/*
	if (lexedTokens.size() != 0)
	{
	lexedTokens.pop_back();
	}
	*/
	
	
	lexedTokens.push_back(newToken);
	
	//tokens++;
}

void StateMachine::StateOne(char temp, ifstream &theFile)
{

	if (theFile.eof())
	{
		Insert("EOF", "");
		return;
	}
	else if (isspace(temp) && temp != '\n') //This was changed to isspace instead of == ' ' when project 3 error was detected
	{ //The && temp != '\n' was added in Project 5
		StateOne(theFile.get(), theFile);
		return;
	}

	tempWhatIs = "";
	tempLineCount = 0;

	bool isNextSpace = false;
	char temp2 = theFile.peek();
	string nextChar = String_Cast(temp2);
	string thisChar = String_Cast(temp);

	if (temp == '\n' || thisChar == "\n")
	{
		AddLineNum();
		StateOne(theFile.get(), theFile);
		return;
	}

	else if (nextChar == " ")
	{
		isNextSpace = true;
	}

	if (thisChar == ",")
	{
		Insert("COMMA", ",");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == ".")
	{
		Insert("PERIOD", ".");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "?")
	{
		Insert("Q_MARK", "?");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "(")
	{
		Insert("LEFT_PAREN", "(");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == ")")
	{
		Insert("RIGHT_PAREN", ")");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == ":")
	{
		if (isNextSpace)
		{
			Insert("COLON", ":");
			StateOne(theFile.get(), theFile);
			return;
		}
		else if (nextChar == "-")
		{
			StateTwo(theFile.get(), theFile);
			return;
		}
		else
		{
			Insert("COLON", ":");
			StateOne(theFile.get(), theFile);
			return;
		}
	}
	else if (thisChar == "*")
	{
		Insert("MULTIPLY", "*");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "+")
	{
		Insert("ADD", "+");
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "'")
	{
		tempWhatIs += String_Cast('\'');
		StateFive('\'', theFile);
		return;
	}
	else if (thisChar == "#")
	{
		tempWhatIs += "#";
		StateFive('#', theFile);
		return;
	}
	else if (LETTERS.find(thisChar) != string::npos)
	{
		if (thisChar == "S")
		{
			tempWhatIs += "S";
			StateThree('S', theFile);
			return;
		}
		else if (thisChar == "F")
		{
			tempWhatIs += "F";
			StateThree('F', theFile);
			return;
		}
		else if (thisChar == "R")
		{
			tempWhatIs += "R";
			StateThree('R', theFile);
			return;
		}
		else if (thisChar == "Q")
		{
			tempWhatIs += "Q";
			StateThree('Q', theFile);
			return;
		}

		tempWhatIs += thisChar;
		StateFour(theFile.get(), theFile);
		return;
	}
	else
	{
		Insert("UNDEFINED", thisChar);
		StateOne(theFile.get(), theFile);
		return;
	}

	cout << endl
		<< "ERROR_1" << endl;
	return;
}

void StateMachine::StateTwo(char temp, ifstream &theFile)
{
	if (String_Cast(temp) == "-")
	{
		Insert("COLON_DASH", ":-");
		StateOne(theFile.get(), theFile);
		return;
	}
	else
	{
		cout << endl
			<< "ERROR_3" << endl;
	}
}

void StateMachine::StateThree(char temp, ifstream &theFile)
{

	string charFeed = String_Cast(temp);
	string thisChar = "";

	if (charFeed == "S")
	{
		thisChar = String_Cast(theFile.get());
		if (thisChar == "c")
		{
			tempWhatIs += "c";
			thisChar = String_Cast(theFile.get());
			if (thisChar == "h")
			{
				tempWhatIs += "h";
				thisChar = String_Cast(theFile.get());
				if (thisChar == "e")
				{
					tempWhatIs += "e";
					thisChar = String_Cast(theFile.get());
					if (thisChar == "m")
					{
						tempWhatIs += "m";
						thisChar = String_Cast(theFile.get());
						if (thisChar == "e")
						{
							tempWhatIs += "e";
							thisChar = String_Cast(theFile.get());
							if (thisChar == "s")
							{
								tempWhatIs += "s";
								if (LETTERS.find(String_Cast(theFile.peek())) != string::npos)
								{
									StateFour(theFile.get(), theFile);
									return;
								}
								Insert("SCHEMES", "Schemes");
								StateOne(theFile.get(), theFile);
								return;
							}
							else
							{
								tempWhatIs += thisChar;
								StateZero(Char_Cast(thisChar), theFile);
								return;
							}
						}
						else
						{
							tempWhatIs += thisChar;
							StateZero(Char_Cast(thisChar), theFile);
							return;
						}
					}
					else
					{
						tempWhatIs += thisChar;
						StateZero(Char_Cast(thisChar), theFile);
						return;
					}
				}
				else
				{
					tempWhatIs += thisChar;
					StateZero(Char_Cast(thisChar), theFile);
					return;
				}
			}
			else
			{
				tempWhatIs += thisChar;
				StateZero(Char_Cast(thisChar), theFile);
				return;
			}
		}
		else
		{
			tempWhatIs += thisChar;
			StateZero(Char_Cast(thisChar), theFile);
			return;
		}
	}
	else if (charFeed == "F")
	{
		thisChar = String_Cast(theFile.get());
		if (thisChar == "a")
		{
			tempWhatIs += "a";
			thisChar = String_Cast(theFile.get());
			if (thisChar == "c")
			{
				tempWhatIs += "c";
				thisChar = String_Cast(theFile.get());
				if (thisChar == "t")
				{
					tempWhatIs += "t";
					thisChar = String_Cast(theFile.get());
					if (thisChar == "s")
					{
						tempWhatIs += "s";
						if (LETTERS.find(String_Cast(theFile.peek())) != string::npos)
						{
							StateFour(theFile.get(), theFile);
							return;
						}
						Insert("FACTS", "Facts");
						StateOne(theFile.get(), theFile);
						return;
					}
					else
					{
						tempWhatIs += thisChar;
						StateZero(Char_Cast(thisChar), theFile);
						return;
					}
				}
				else
				{
					tempWhatIs += thisChar;
					StateZero(Char_Cast(thisChar), theFile);
					return;
				}
			}
			else
			{
				tempWhatIs += thisChar;
				StateZero(Char_Cast(thisChar), theFile);
				return;
			}
		}
		else
		{
			tempWhatIs += thisChar;
			StateZero(Char_Cast(thisChar), theFile);
			return;
		}
	}
	else if (charFeed == "R")
	{
		thisChar = String_Cast(theFile.get());
		if (thisChar == "u")
		{
			tempWhatIs += "u";
			thisChar = String_Cast(theFile.get());
			if (thisChar == "l")
			{
				tempWhatIs += "l";
				thisChar = String_Cast(theFile.get());
				if (thisChar == "e")
				{
					tempWhatIs += "e";
					thisChar = String_Cast(theFile.get());
					if (thisChar == "s")
					{
						tempWhatIs += "s";
						if (LETTERS.find(String_Cast(theFile.peek())) != string::npos)
						{
							StateFour(theFile.get(), theFile);
							return;
						}
						Insert("RULES", "Rules");
						StateOne(theFile.get(), theFile);
						return;
					}
					else
					{
						tempWhatIs += thisChar;
						StateZero(Char_Cast(thisChar), theFile);
						return;
					}
				}
				else
				{
					tempWhatIs += thisChar;
					StateZero(Char_Cast(thisChar), theFile);
					return;
				}
			}
			else
			{
				tempWhatIs += thisChar;
				StateZero(Char_Cast(thisChar), theFile);
				return;
			}
		}
		else
		{
			tempWhatIs += thisChar;
			StateZero(Char_Cast(thisChar), theFile);
			return;
		}
	}
	else if (charFeed == "Q")
	{
		thisChar = String_Cast(theFile.get());
		if (thisChar == "u")
		{
			tempWhatIs += "u";
			thisChar = String_Cast(theFile.get());
			if (thisChar == "e")
			{
				tempWhatIs += "e";
				thisChar = String_Cast(theFile.get());
				if (thisChar == "r")
				{
					tempWhatIs += "r";
					thisChar = String_Cast(theFile.get());
					if (thisChar == "i")
					{
						tempWhatIs += "i";
						thisChar = String_Cast(theFile.get());
						if (thisChar == "e")
						{
							tempWhatIs += "e";
							thisChar = String_Cast(theFile.get());
							if (thisChar == "s")
							{
								tempWhatIs += "s";
								if (LETTERS.find(String_Cast(theFile.peek())) != string::npos)
								{
									StateFour(theFile.get(), theFile);
									return;
								}
								Insert("QUERIES", "Queries");
								StateOne(theFile.get(), theFile);
								return;
							}
							else
							{
								tempWhatIs += thisChar;
								StateZero(Char_Cast(thisChar), theFile);
								return;
							}
						}
						else
						{
							tempWhatIs += thisChar;
							StateZero(Char_Cast(thisChar), theFile);
							return;
						}
					}
					else
					{
						tempWhatIs += thisChar;
						StateZero(Char_Cast(thisChar), theFile);
						return;
					}
				}
				else
				{
					tempWhatIs += thisChar;
					StateZero(Char_Cast(thisChar), theFile);
					return;
				}
			}
			else
			{
				tempWhatIs += thisChar;
				StateZero(Char_Cast(thisChar), theFile);
				return;
			}
		}
		else
		{
			tempWhatIs += thisChar;
			StateZero(Char_Cast(thisChar), theFile);
			return;
		}
	}
}

void StateMachine::StateZero(char temp, ifstream &theFile)
{

	string thisChar = String_Cast(temp);

	if (theFile.eof())
	{
		Insert("ID", tempWhatIs);
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "\n" || temp == '\n')
	{
		Insert("ID", tempWhatIs.substr(0, tempWhatIs.size() - 1)); //isRecent
		AddLineNum();
		StateOne(theFile.get(), theFile); //isRecent
										  //StateFour(theFile.get(), theFile); //IsRecently_Commented-out
		return;
	}
	else if (thisChar == " " || temp == ' ')
	{
		Insert("ID", tempWhatIs.substr(0, tempWhatIs.size()));
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (LETTERS.find(thisChar) == string::npos && (!isdigit(temp)))
	{
		Insert("ID", tempWhatIs.substr(0, tempWhatIs.size() - 1));
		StateOne(temp, theFile);
		return;
	}
	else
	{
		StateFour(theFile.get(), theFile); //NOT having a tempWhatIs += thisChar like StateFour has. This solves the "Fin" problem
		return;
	}
}

void StateMachine::StateFour(char temp, ifstream &theFile)
{

	string thisChar = String_Cast(temp);

	if (theFile.eof())
	{
		Insert("ID", tempWhatIs);
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "\n" || temp == '\n')
	{
		Insert("ID", tempWhatIs); //isRecent
		AddLineNum();
		StateOne(theFile.get(), theFile); //isRecent
										  //StateFour(theFile.get(), theFile); //IsRecently_Commented-out
		return;
	}
	else if (thisChar == " " || temp == ' ')
	{
		Insert("ID", tempWhatIs.substr(0, tempWhatIs.size()));
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (LETTERS.find(thisChar) == string::npos && (!isdigit(temp)))
	{
		Insert("ID", tempWhatIs.substr(0, tempWhatIs.size()));
		StateOne(temp, theFile);
		return;
	}
	else
	{
		tempWhatIs += thisChar;
		StateFour(theFile.get(), theFile);
		return;
	}
	cout << endl
		<< "ERROR 2" << endl;
}

void StateMachine::StateFive(char temp, ifstream &theFile)
{
	//It will never get to theFile.eof() in this function
	string thisChar = String_Cast(temp);

	if (thisChar == "'")
	{
		tempWhatIs = "'";
		StateSix(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "#")
	{
		tempWhatIs = "#";
		if (String_Cast(theFile.peek()) == "|")
		{
			theFile.get();
			tempWhatIs += "|";
			StateEight(theFile.get(), theFile);
			return;
		}
		StateSeven(theFile.get(), theFile);
		return;
	}
}

void StateMachine::StateSix(char temp, ifstream &theFile)
{

	string thisChar = String_Cast(temp);
	if (theFile.eof())
	{
		Insert("UNDEFINED", tempWhatIs);
		lineNum += tempLineCount;
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (thisChar == "'")
	{
		if (tempWhatIs.size() > 0)
		{
			if (String_Cast(theFile.peek()) == "'")
			{
				theFile.get();
				tempWhatIs += "''";
				StateSix(theFile.get(), theFile);
				return;
			}
		}
		tempWhatIs += "'";
		Insert("STRING", tempWhatIs);
		lineNum += tempLineCount;
		StateOne(theFile.get(), theFile);
		return;
	}
	else
	{
		if (thisChar == "\n")
		{
			tempLineCount++;
		}
		tempWhatIs += thisChar;
		StateSix(theFile.get(), theFile);
		return;
	}
}

void StateMachine::StateSeven(char temp, ifstream &theFile)
{
	string thisChar = String_Cast(temp);
	if (theFile.eof())
	{
		Insert("COMMENT", tempWhatIs);
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (temp == '\n')
	{
		Insert("COMMENT", tempWhatIs);
		AddLineNum();
		StateOne(theFile.get(), theFile);
		return;
	}
	else
	{
		tempWhatIs += thisChar;
		StateSeven(theFile.get(), theFile);
		return;
	}
}

void StateMachine::StateEight(char temp, ifstream &theFile)
{

	if (theFile.eof())
	{

		Insert("UNDEFINED", tempWhatIs);
		lineNum += tempLineCount;
		StateOne(theFile.get(), theFile);
		return;
	}
	else if (String_Cast(temp) == "|" && String_Cast(theFile.peek()) == "#")
	{
		tempWhatIs += "|#";
		theFile.get();
		Insert("COMMENT", tempWhatIs);
		lineNum += tempLineCount;
		StateOne(theFile.get(), theFile);
		return;
	}
	else
	{
		if (String_Cast(temp) == "\n")
		{
			tempLineCount++;
		}
		tempWhatIs += String_Cast(temp);
		StateEight(theFile.get(), theFile);
		return;
	}
}

vector<string> StateMachine::GetOutput()
{
	return lexedTokens;
}

int StateMachine::GetDebug()
{
	return debug;
}