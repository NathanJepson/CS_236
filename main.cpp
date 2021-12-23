#include "StateMachine.h"
#include "Tokens.h"
#include "DatalogProgram.h"
#include "Interpreter.h"

using namespace std;


vector<string> ProjectOne(string filestring)
{
	vector<string> myTokens;
	ostringstream giveMeOutput;

	ifstream myfile;
	myfile.open(filestring); //Switch to filestring or it gives error with argv[1]
	StateMachine Lexer;

	if (myfile.is_open())
	{
		char fileChar = myfile.get();
		Lexer.StateOne(fileChar, myfile);
	}
	myfile.close();
	Tokens lexed;
	vector<string> theTokens = Lexer.GetOutput();
	for (unsigned i = 0; i < theTokens.size(); i++)
	{
		lexed.push_back(theTokens.at(i));
	}

	for (unsigned i = 0; i < lexed.size(); i++)
	{
		//cout << myTokens.at(i);
		giveMeOutput << lexed.at(i);
	}
	giveMeOutput << "Total Tokens = " << lexed.size() << endl; //NOT USED
	
	
	//ofstream outfile;
	//ofstream ofs;
	//ofs.open("lexed.txt", std::ofstream::out | std::ofstream::trunc); //erases contents of parsed.txt
	//ofs.close();

	//outfile.open("lexed.txt");
	//outfile << giveMeOutput.str();
	//outfile.close();
	
	return lexed;
}

int main(int argc, char *argv[])
{
	//string filestring = argv[1]; //FIXME
	vector<string> myTokens = ProjectOne("in_file.txt"); //Put potential program in "in_file.txt"
	//////////PROJECT 2///////////////////////////////////////////
	//ofs.open("parsed.txt", std::ofstream::out | std::ofstream::trunc); //erases contents of parsed.txt
	//ofs.close();
	DatalogProgram datalogProgram(myTokens);
	datalogProgram.Parse();
	bool isValid = datalogProgram.isItValid();
	//datalogProgram.WriteToConsole();
	//////////////////////////////////////////////////////////////

	//PROJECT 3///////////////////////////////////////////////////
	if (isValid == false)
	{
		datalogProgram.WriteToConsole();
	}
	else
	{
		Interpreter intptr;
		intptr.Acquire_DatalogProgram(datalogProgram.Get_Schemes(), datalogProgram.Get_Facts(),
			datalogProgram.Get_Rules(), datalogProgram.Get_Queries(), datalogProgram.Get_Domain());
		
		intptr.Evaluate_Graph(0);
		intptr.Evaluate_Queries(0);
	}

	system("pause"); //Also comment ofstream
	return 0;
}


