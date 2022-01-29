#include "SourceProcessor.h"

// method for processing the source program
// This method currently only inserts the procedure name into the database
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all the required syntax.
void SourceProcessor::process(string program) {
	// initialize the database
	Database::initialize();

	// tokenize the program
	Tokenizer tk;
	vector<string> tokens;
	tk.tokenize(program, tokens);
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the programs are valid.

	int i = 0; // use for iterating through the vector token
	int countlines = 1; // to keep track of which line in the source for storing into database for Assignment, Print, Read, Statement


	while (i < tokens.size())
	{
		string constant = tokens.at(i);
		stringstream geek(constant);
		int c = 0;
		geek >> c;

		if (tokens.at(i) == "procedure")
		{
			string procedureName = tokens.at(i += 1);
			Database::insertProcedure(procedureName); // insert the procedure into the database
			i++;
		}
		else if (tokens.at(i) == "read")
		{
			string variableName = tokens.at(i += 1);
			Database::insertVariable(variableName); // insert the variable into the database
			string readLine;
			stringstream ss;
			ss << countlines;
			ss >> readLine;
			Database::insertRead(readLine); // insert current line into the database under reads
			i++;
		}
		else if (tokens.at(i) == "=")
		{
			string variableName = tokens.at(i - 1);
			Database::insertVariable(variableName); // insert the variable into the database

			string assignmentLine;
			stringstream ss;
			ss << countlines;
			ss >> assignmentLine;
			// assignmentLine = to_string(countlines);
			Database::insertAssignment(assignmentLine); // insert current line into the database under assignments
			i++;
		}
		else if (tokens.at(i) == ";")
		{
			string statementLine;
			stringstream ss;
			ss << countlines;
			ss >> statementLine;
			Database::insertStatement(statementLine);

			countlines++;
			i++;
		}
		else if (tokens.at(i) == "print")
		{
			string printLine;
			stringstream ss;
			ss << countlines;
			ss >> printLine;
			Database::insertPrint(printLine); // insert current line into the database under prints
			i++;
		}
		else if (10 > c && c > 0)
		{
			string constantName;
			stringstream ss;
			ss << c;
			ss >> constantName;
			Database::insertConstant(constantName);
			i++;
		}
		else
		{
			i++;
		}
	}
	
}