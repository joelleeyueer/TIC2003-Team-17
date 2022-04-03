#include "SourceProcessor.h"
#include "database.h"

#include <regex>
#include <iostream>
#include <stack>

#define INTEGER_PATTERN "[0-9]+"
#define NAME_PATTERN "[a-zA-Z][a-zA-Z0-9]*"
#define IDENT_PATTERN "[a-zA-Z][a-zA-Z0-9]*"

// using namespace std;

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

	list<string> ltokens;
	int i = 0;
	while (i < tokens.size())
	{
		string insert = tokens.at(i);
		ltokens.push_back(insert);
		i++;
	}


	parse(ltokens);

}

void SourceProcessor::parse(list<string> tokens) {
	remainingTokens = tokens;

	try {
		parseProgram();
	}
	catch (const std::exception& ex) {
		//std::throw_with_nested("error in parsing query");
	}
}

void SourceProcessor::expect(string symbol)
{
	if (match(symbol)) {
		next();
		return;
	}

	//std::throw_with_nested("expected: '" + symbol + "', got '" + remainingTokens.front() + "' instead");
}

bool SourceProcessor::match(string symbol)
{
	if (symbol == remainingTokens.front()) {
		return 1;
	}

	return 0;
}

void SourceProcessor::next()
{
	if (!remainingTokens.empty()) {
		remainingTokens.pop_front();
	}
}

bool SourceProcessor::checkName(string token)
{
	return regex_match(token, regex(NAME_PATTERN));
}

bool SourceProcessor::checkInteger(string token)
{
	return regex_match(token, regex(INTEGER_PATTERN));
}


void SourceProcessor::parseProgram()
{
	try {
		parseProcedure();

	}
	catch (const std::exception& ex) {
		//std::throw_with_nested("error in declaration list");
	}
}

int countlines = 1;
int currentlines = 1;
int prevlines = 0;
int countparent = 0;
int parentChild = 0; // it is not a child, = 1 if it is a child
int prevCountparent = 0;
int isnested = 0; // it is not nested, > 1 if it is nested
vector<int> ancestors; // vector of parents to track previous parents
string lhs;
string rhs;
vector<string> procedureList; // vector of procedures stored
vector<vector<string>> procedureCalls; // vector of vector of procedures to accomodate callst
vector<string> procedureTemp; // temporary vector to store current procedure calls, transferred to procedureCalls vector
vector<string> currentproc;
vector<string> prevproc;
vector<string> whilesproc;
vector<string> whilesvector;
vector<string> thenvector;
vector<string> elsevector;
vector<vector<string>> nextst;
vector<string> nextstTemp;
vector<string> nextstInsert;

void SourceProcessor::parseProcedure()
{
	if (remainingTokens.front() == "procedure")
	{
		next();
		if (checkName(remainingTokens.front())) // checks if it is a NAME grammer
		{
			Database::insertProcedure(remainingTokens.front()); // insert the procedure into the database
			procedureList.push_back(remainingTokens.front()); // store the procedure into vector
			procedureTemp.push_back(remainingTokens.front()); // store the procedure into temp vector
			next();
			expect("{");
			parseStatement();
		}
		else
		{
			//std::throw_with_nested("error in parseProcedure");
		}
	}

}

void SourceProcessor::parseStatement()
{
	while (remainingTokens.front() != "}")
	{

		if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
		{
			string parentLine;
			stringstream pp;
			pp << countparent;
			pp >> parentLine;

			string childLine;
			stringstream cc;
			cc << countlines;
			cc >> childLine;

			//string grandparentLine;
			//stringstream gg;
			//gg << parentChild;
			//gg >> grandparentLine;

			Database::insertChild(parentLine, childLine); // parent table
			//Database::insertGrandchild(grandparentLine, childLine); // parent* table
			for (int i = 0; i < ancestors.size(); i++)
			{
				string grandparentLine;
				stringstream gg;
				gg << ancestors.at(i);
				gg >> grandparentLine;

				Database::insertGrandchild(grandparentLine, childLine); // parent* table

			}
		}
		else if (countparent > 0 && parentChild == 0) // if no nesting of while or if
		{
			string parentLine;
			stringstream pp;
			pp << countparent;
			pp >> parentLine;

			string childLine;
			stringstream cc;
			cc << countlines;
			cc >> childLine;

			Database::insertChild(parentLine, childLine); // parent table
			//Database::insertGrandchild(parentLine, childLine); // parent* table
			for (int i = 0; i < ancestors.size(); i++)
			{
				string grandparentLine;
				stringstream gg;
				gg << ancestors.at(i);
				gg >> grandparentLine;

				Database::insertGrandchild(grandparentLine, childLine); // parent* table

			}
		}

		string statementLine;
		stringstream ss;
		ss << countlines;
		ss >> statementLine;
		Database::insertStatement(statementLine);
		Database::insertProcstmt(procedureList.back() , statementLine);

		if (remainingTokens.front() == "while")
		{

			if (countparent > 0) // means it is nested
			{
				isnested += 1;
			}

			parentChild = countparent;
			prevCountparent = countparent;
			countparent = countlines;
			ancestors.push_back(countparent);
			string whileLine;
			stringstream ss;
			ss << countlines;
			ss >> whileLine;
			Database::insertWhile(whileLine);
			

			string prevLine;
			string currentLine;
			stringstream pp;
			stringstream cc;
			prevlines = currentlines - 1;
			pp << prevlines;
			pp >> prevLine;
			cc << currentlines;
			cc >> currentLine;
			Database::getProcstmt(currentproc, currentLine);
			Database::getProcstmt(prevproc, prevLine);

			//cout << prevproc.back() << currentproc.back() << endl;

			if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
			{
				if (prevlines != 0 && prevproc.back() == currentproc.back())
				{
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
				}
			}

			if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
			{
				if (prevproc.back() == currentproc.back())
				{
					Database::insertNexts(elsevector.back(), currentLine);
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
					elsevector.pop_back();
				}
			}

			if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
			{
				Database::insertNexts(elsevector.back(), whilesvector.back());
				nextstTemp.push_back(elsevector.back());
				nextstTemp.push_back(whilesvector.back());
				elsevector.pop_back();
			}

			if (!whilesvector.empty()) // while statement
			{
				Database::insertNexts(prevLine, whilesvector.back());
				nextstTemp.push_back(prevLine);
				nextstTemp.push_back(whilesvector.back());
				Database::getProcstmt(whilesproc, whilesvector.back());
				if (whilesproc.back() == currentproc.back())
				{
					Database::insertNexts(whilesvector.back(), currentLine);
					nextstTemp.push_back(whilesvector.back());
					nextstTemp.push_back(currentLine);
					whilesvector.pop_back();
				}
			}

			if (!thenvector.empty()) // if-then statment
			{
				Database::insertNexts(thenvector.back(), currentLine);
				nextstTemp.push_back(thenvector.back());
				nextstTemp.push_back(currentLine);
				thenvector.pop_back();
			}

			elsevector.clear();
			whilesvector.clear();
			thenvector.clear();

			next();
			expect("(");
			list<string> conditionTokens = remainingTokens;
			list<string> nestedconditionTokens = remainingTokens;
			parseFactorCondition();
			expect("{");
			countlines++;
			currentlines++;
			parseStatement();
			whilesvector.push_back(whileLine);
			ancestors.pop_back();

			if (isnested == 0)
			{
				countparent = 0;
				parentChild = 0;
			}
			else if (isnested > 0)
			{
				countparent = prevCountparent;
				parentChild = 0;
				isnested -= 1;
			}

			while (conditionTokens.front() != ")")
			{
				if (checkName(conditionTokens.front())) // if it is a variable
				{
					string useVariableToken = conditionTokens.front();
					Database::insertUses(whileLine, useVariableToken);
					Database::insertUsesproc(procedureList.back(), useVariableToken);
				}
				conditionTokens.pop_front();
			}

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				//string parentLine;
				//stringstream pp;
				//pp << countparent;
				//pp >> parentLine;

				//string grandparentLine;
				//stringstream gg;
				//gg << parentChild;
				//gg >> grandparentLine;

				while (nestedconditionTokens.front() != ")")
				{
					if (checkName(nestedconditionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedconditionTokens.front();
						//Database::insertUses(parentLine, useVariableToken);
						//Database::insertUses(grandparentLine, useVariableToken);
						for (int i = 0; i < ancestors.size(); i++)
						{
							string ancestorsLine;
							stringstream gg;
							gg << ancestors.at(i);
							gg >> ancestorsLine;

							Database::insertUses(ancestorsLine, useVariableToken);
						}
					}
					nestedconditionTokens.pop_front();
				}

			}
			else if (countparent > 0 && parentChild == 0) // if no nesting of while or if
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string childLine;
				stringstream cc;
				cc << countlines;
				cc >> childLine;

				while (nestedconditionTokens.front() != ")")
				{
					if (checkName(nestedconditionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedconditionTokens.front();
						Database::insertUses(parentLine, useVariableToken);
					}
					nestedconditionTokens.pop_front();
				}

			}


		}
		else if (remainingTokens.front() == "if")
		{

			if (countparent > 0) // means it is nested
			{
				isnested += 1;
			}

			parentChild = countparent;
			prevCountparent = countparent;
			countparent = countlines;
			ancestors.push_back(countparent);
			string ifLine;
			stringstream ss;
			ss << countlines;
			ss >> ifLine;
			Database::insertIf(ifLine);

			string prevLine;
			string currentLine;
			stringstream pp;
			stringstream cc;
			prevlines = currentlines - 1;
			pp << prevlines;
			pp >> prevLine;
			cc << currentlines;
			cc >> currentLine;
			Database::getProcstmt(currentproc, currentLine);
			Database::getProcstmt(prevproc, prevLine);

			//cout << prevproc.back() << currentproc.back() << endl;

			if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
			{
				if (prevlines != 0 && prevproc.back() == currentproc.back())
				{
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
				}
			}

			if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
			{
				if (prevproc.back() == currentproc.back())
				{
					Database::insertNexts(elsevector.back(), currentLine);
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
					elsevector.pop_back();
				}
			}

			if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
			{
				Database::insertNexts(elsevector.back(), whilesvector.back());
				nextstTemp.push_back(elsevector.back());
				nextstTemp.push_back(whilesvector.back());
				elsevector.pop_back();
			}

			if (!whilesvector.empty()) // while statement
			{
				Database::insertNexts(prevLine, whilesvector.back());
				nextstTemp.push_back(prevLine);
				nextstTemp.push_back(whilesvector.back());
				Database::getProcstmt(whilesproc, whilesvector.back());
				if (whilesproc.back() == currentproc.back())
				{
					Database::insertNexts(whilesvector.back(), currentLine);
					nextstTemp.push_back(whilesvector.back());
					nextstTemp.push_back(currentLine);
					whilesvector.pop_back();
				}
			}

			if (!thenvector.empty()) // if-then statment
			{
				Database::insertNexts(thenvector.back(), currentLine);
				nextstTemp.push_back(thenvector.back());
				nextstTemp.push_back(currentLine);
				thenvector.pop_back();
			}

			elsevector.clear();
			whilesvector.clear();
			thenvector.clear();

			next();
			expect("(");
			list<string> conditionTokens = remainingTokens;
			list<string> nestedconditionTokens = remainingTokens;
			parseFactorCondition();
			expect("then");
			expect("{");
			countlines++;
			currentlines++;
			parseStatement();
			string lastthenline;
			stringstream ll;
			ll << currentlines - 1;
			ll >> lastthenline;
			thenvector.push_back(ifLine);


			if (remainingTokens.front() == "else")
			{
				next();
				expect("{");
				parseStatement();
				elsevector.push_back(lastthenline);
			}

			ancestors.pop_back();

			if (isnested == 0)
			{
				countparent = 0;
				parentChild = 0;
			}
			else if (isnested > 0)
			{
				countparent = prevCountparent;
				parentChild = 0;
				isnested -= 1;
			}

			while (conditionTokens.front() != ")")
			{
				if (checkName(conditionTokens.front())) // if it is a variable
				{
					string useVariableToken = conditionTokens.front();
					Database::insertUses(ifLine, useVariableToken);
					Database::insertUsesproc(procedureList.back(), useVariableToken);
				}
				conditionTokens.pop_front();
			}

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				//string parentLine;
				//stringstream pp;
				//pp << countparent;
				//pp >> parentLine;

				//string grandparentLine;
				//stringstream gg;
				//gg << parentChild;
				//gg >> grandparentLine;

				while (nestedconditionTokens.front() != ")")
				{
					if (checkName(nestedconditionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedconditionTokens.front();
						//Database::insertUses(parentLine, useVariableToken);
						//Database::insertUses(grandparentLine, useVariableToken);
						for (int i = 0; i < ancestors.size(); i++)
						{
							string ancestorsLine;
							stringstream gg;
							gg << ancestors.at(i);
							gg >> ancestorsLine;

							Database::insertUses(ancestorsLine, useVariableToken);
						}
					}
					nestedconditionTokens.pop_front();
				}

			}
			else if (countparent > 0 && parentChild == 0) // if no nesting of while or if
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string childLine;
				stringstream cc;
				cc << countlines;
				cc >> childLine;

				while (nestedconditionTokens.front() != ")")
				{
					if (checkName(nestedconditionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedconditionTokens.front();
						Database::insertUses(parentLine, useVariableToken);
					}
					nestedconditionTokens.pop_front();
				}

			}



		}
		else if (remainingTokens.front() == "call")
		{
			next();
			if (checkName(remainingTokens.front())) // checks if it is a NAME grammer
			{
				Database::insertCalls(procedureList.back(), remainingTokens.front()); // insert direct calls into database
				procedureTemp.push_back(remainingTokens.front()); // store its procedure call in the same temp vector
				next();
				expect(";");

				string prevLine;
				string currentLine;
				stringstream pp;
				stringstream cc;
				prevlines = currentlines - 1;
				pp << prevlines;
				pp >> prevLine;
				cc << currentlines;
				cc >> currentLine;
				Database::getProcstmt(currentproc, currentLine);
				Database::getProcstmt(prevproc, prevLine);

				//cout << prevproc.back() << currentproc.back() << endl;

				if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
				{
					if (prevlines != 0 && prevproc.back() == currentproc.back())
					{
						Database::insertNexts(prevLine, currentLine);
						nextstTemp.push_back(prevLine);
						nextstTemp.push_back(currentLine);
					}
				}

				if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
				{
					if (prevproc.back() == currentproc.back())
					{
						Database::insertNexts(elsevector.back(), currentLine);
						Database::insertNexts(prevLine, currentLine);
						nextstTemp.push_back(elsevector.back());
						nextstTemp.push_back(currentLine);
						nextstTemp.push_back(prevLine);
						nextstTemp.push_back(currentLine);
						elsevector.pop_back();
					}
				}

				if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
				{
					Database::insertNexts(elsevector.back(), whilesvector.back());
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(whilesvector.back());
					elsevector.pop_back();
				}

				if (!whilesvector.empty()) // while statement
				{
					Database::insertNexts(prevLine, whilesvector.back());
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(whilesvector.back());
					Database::getProcstmt(whilesproc, whilesvector.back());
					if (whilesproc.back() == currentproc.back())
					{
						Database::insertNexts(whilesvector.back(), currentLine);
						nextstTemp.push_back(whilesvector.back());
						nextstTemp.push_back(currentLine);
						whilesvector.pop_back();
					}
				}

				if (!thenvector.empty()) // if-then statment
				{
					Database::insertNexts(thenvector.back(), currentLine);
					nextstTemp.push_back(thenvector.back());
					nextstTemp.push_back(currentLine);
					thenvector.pop_back();
				}

				elsevector.clear();
				whilesvector.clear();
				thenvector.clear();
				

				countlines++;
				currentlines++;

			}
		}
		else if (remainingTokens.front() == "read")
		{

			string readLine;
			stringstream ss;
			ss << countlines;
			ss >> readLine;

			Database::insertRead(readLine);

			string modifyLine;
			stringstream mm;
			mm << countlines;
			mm >> modifyLine;

			string prevLine;
			string currentLine;
			stringstream pp;
			stringstream cc;
			prevlines = currentlines - 1;
			pp << prevlines;
			pp >> prevLine;
			cc << currentlines;
			cc >> currentLine;
			Database::getProcstmt(currentproc, currentLine);
			Database::getProcstmt(prevproc, prevLine);

			//cout << prevproc.back() << currentproc.back() << endl;

			if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
			{
				if (prevlines != 0 && prevproc.back() == currentproc.back())
				{
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
				}
			}

			if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
			{
				if (prevproc.back() == currentproc.back())
				{
					Database::insertNexts(elsevector.back(), currentLine);
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
					elsevector.pop_back();
				}
			}

			if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
			{
				Database::insertNexts(elsevector.back(), whilesvector.back());
				nextstTemp.push_back(elsevector.back());
				nextstTemp.push_back(whilesvector.back());
				elsevector.pop_back();
			}

			if (!whilesvector.empty()) // while statement
			{
				Database::insertNexts(prevLine, whilesvector.back());
				nextstTemp.push_back(prevLine);
				nextstTemp.push_back(whilesvector.back());
				Database::getProcstmt(whilesproc, whilesvector.back());
				if (whilesproc.back() == currentproc.back())
				{
					Database::insertNexts(whilesvector.back(), currentLine);
					nextstTemp.push_back(whilesvector.back());
					nextstTemp.push_back(currentLine);
					whilesvector.pop_back();
				}
			}

			if (!thenvector.empty()) // if-then statment
			{
				Database::insertNexts(thenvector.back(), currentLine);
				nextstTemp.push_back(thenvector.back());
				nextstTemp.push_back(currentLine);
				thenvector.pop_back();
			}

			elsevector.clear();
			whilesvector.clear();
			thenvector.clear();

			next();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect(";");
			Database::insertModifies(modifyLine, variableToken);
			Database::insertModifiesproc(procedureList.back(), variableToken);

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				//string parentLine;
				//stringstream pp;
				//pp << countparent;
				//pp >> parentLine;

				//string grandparentLine;
				//stringstream gg;
				//gg << parentChild;
				//gg >> grandparentLine;

				//Database::insertModifies(parentLine, variableToken); // 
				//Database::insertModifies(grandparentLine, variableToken); // 
				for (int i = 0; i < ancestors.size(); i++)
				{
					string ancestorsLine;
					stringstream gg;
					gg << ancestors.at(i);
					gg >> ancestorsLine;

					Database::insertModifies(ancestorsLine, variableToken);
				}
			}
			else if (countparent > 0) // if no nesting of while or if
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string childLine;
				stringstream cc;
				cc << countlines;
				cc >> childLine;

				Database::insertModifies(parentLine, variableToken); // 
			}

			countlines++;
			currentlines++;

		}
		else if (remainingTokens.front() == "print")
		{
			string printLine;
			stringstream ss;
			ss << countlines;
			ss >> printLine;

			Database::insertPrint(printLine);

			string useLine;
			stringstream uu;
			uu << countlines;
			uu >> useLine;

			string prevLine;
			string currentLine;
			stringstream pp;
			stringstream cc;
			prevlines = currentlines - 1;
			pp << prevlines;
			pp >> prevLine;
			cc << currentlines;
			cc >> currentLine;
			Database::getProcstmt(currentproc, currentLine);
			Database::getProcstmt(prevproc, prevLine);

			//cout << prevproc.back() << currentproc.back() << endl;

			if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
			{
				if (prevlines != 0 && prevproc.back() == currentproc.back())
				{
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
				}
			}

			if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
			{
				if (prevproc.back() == currentproc.back())
				{
					Database::insertNexts(elsevector.back(), currentLine);
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
					elsevector.pop_back();
				}
			}

			if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
			{
				Database::insertNexts(elsevector.back(), whilesvector.back());
				nextstTemp.push_back(elsevector.back());
				nextstTemp.push_back(whilesvector.back());
				elsevector.pop_back();
			}

			if (!whilesvector.empty()) // while statement
			{
				Database::insertNexts(prevLine, whilesvector.back());
				nextstTemp.push_back(prevLine);
				nextstTemp.push_back(whilesvector.back());
				Database::getProcstmt(whilesproc, whilesvector.back());
				if (whilesproc.back() == currentproc.back())
				{
					Database::insertNexts(whilesvector.back(), currentLine);
					nextstTemp.push_back(whilesvector.back());
					nextstTemp.push_back(currentLine);
					whilesvector.pop_back();
				}
			}

			if (!thenvector.empty()) // if-then statment
			{
				Database::insertNexts(thenvector.back(), currentLine);
				nextstTemp.push_back(thenvector.back());
				nextstTemp.push_back(currentLine);
				thenvector.pop_back();
			}

			elsevector.clear();
			whilesvector.clear();
			thenvector.clear();

			next();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect(";");
			Database::insertUses(useLine, variableToken);
			Database::insertUsesproc(procedureList.back(), variableToken);

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				//string parentLine;
				//stringstream pp;
				//pp << countparent;
				//pp >> parentLine;

				//string grandparentLine;
				//stringstream gg;
				//gg << parentChild;
				//gg >> grandparentLine;

				//Database::insertUses(parentLine, variableToken); // 
				//Database::insertUses(grandparentLine, variableToken); // 
				for (int i = 0; i < ancestors.size(); i++)
				{
					string ancestorsLine;
					stringstream gg;
					gg << ancestors.at(i);
					gg >> ancestorsLine;

					Database::insertUses(ancestorsLine, variableToken);
				}
			}
			else if (countparent > 0) // if no nesting of while or if
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string childLine;
				stringstream cc;
				cc << countlines;
				cc >> childLine;

				Database::insertUses(parentLine, variableToken); // 
			}

			countlines++;
			currentlines++;

		}
		else // it is an assignment
		{
			string assignmentLine;
			stringstream ss;
			ss << countlines;
			ss >> assignmentLine;

			string modifyLine;
			stringstream mm;
			mm << countlines;
			mm >> modifyLine;

			string useLine;
			stringstream uu;
			uu << countlines;
			uu >> useLine;

			string prevLine;
			string currentLine;
			stringstream pp;
			stringstream cc;
			prevlines = currentlines - 1;
			pp << prevlines;
			pp >> prevLine;
			cc << currentlines;
			cc >> currentLine;
			Database::getProcstmt(currentproc, currentLine);
			Database::getProcstmt(prevproc, prevLine);

			//cout << prevproc.back() << currentproc.back() << endl;

			if (whilesvector.empty() && thenvector.empty() && elsevector.empty()) // normal next with no if or while
			{
				if (prevlines != 0 && prevproc.back() == currentproc.back())
				{
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
				}
			}

			if (!elsevector.empty() && whilesvector.empty()) // if-else statement not nested in while
			{
				if (prevproc.back() == currentproc.back())
				{
					Database::insertNexts(elsevector.back(), currentLine);
					Database::insertNexts(prevLine, currentLine);
					nextstTemp.push_back(elsevector.back());
					nextstTemp.push_back(currentLine);
					nextstTemp.push_back(prevLine);
					nextstTemp.push_back(currentLine);
					elsevector.pop_back();
				}
			}

			if (!elsevector.empty() && !whilesvector.empty()) // if-else statement nested in while
			{
				Database::insertNexts(elsevector.back(), whilesvector.back());
				nextstTemp.push_back(elsevector.back());
				nextstTemp.push_back(whilesvector.back());
				elsevector.pop_back();
			}

			if (!whilesvector.empty()) // while statement
			{
				Database::insertNexts(prevLine, whilesvector.back());
				nextstTemp.push_back(prevLine);
				nextstTemp.push_back(whilesvector.back());
				Database::getProcstmt(whilesproc, whilesvector.back());
				if (whilesproc.back() == currentproc.back())
				{
					Database::insertNexts(whilesvector.back(), currentLine);
					nextstTemp.push_back(whilesvector.back());
					nextstTemp.push_back(currentLine);
					whilesvector.pop_back();
				}
			}

			if (!thenvector.empty()) // if-then statment
			{
				Database::insertNexts(thenvector.back(), currentLine);
				nextstTemp.push_back(thenvector.back());
				nextstTemp.push_back(currentLine);
				thenvector.pop_back();
			}

			elsevector.clear();
			whilesvector.clear();
			thenvector.clear();


			lhs = remainingTokens.front();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect("=");
			list<string> expressionTokens = remainingTokens;
			//list<string> postfixexpressionTokens;
			parseExpression();
			rhs += expr;
			expr = " ";
			list<string> nestedexpressionTokens = remainingTokens;
			parseFactor(); // factor can be either a variableName, Constant, Expre

			while (expressionTokens.front() != ";")
			{
				//rhs += expressionTokens.front();
				//rhs += postfixexpressionTokens.front();
				if (checkName(expressionTokens.front())) // if it is a variable
				{
					string useVariableToken = expressionTokens.front();
					Database::insertUses(useLine, useVariableToken);
					Database::insertUsesproc(procedureList.back(), useVariableToken);
				}
				expressionTokens.pop_front();
				//postfixexpressionTokens.pop_front();
			}

			Database::insertAssignment(assignmentLine, lhs, rhs);
			rhs = " ";
			Database::insertModifies(modifyLine, variableToken);
			Database::insertModifiesproc(procedureList.back(), variableToken);

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				//string parentLine;
				//stringstream pp;
				//pp << countparent;
				//pp >> parentLine;

				//string grandparentLine;
				//stringstream gg;
				//gg << parentChild;
				//gg >> grandparentLine;

				//Database::insertModifies(parentLine, variableToken);
				//Database::insertModifies(grandparentLine, variableToken);
				for (int i = 0; i < ancestors.size(); i++)
				{
					string ancestorsLine;
					stringstream gg;
					gg << ancestors.at(i);
					gg >> ancestorsLine;

					Database::insertModifies(ancestorsLine, variableToken);
				}

				while (nestedexpressionTokens.front() != ";")
				{

					if (checkName(nestedexpressionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedexpressionTokens.front();
						//Database::insertUses(parentLine, useVariableToken);
						//Database::insertUses(grandparentLine, useVariableToken);
						for (int i = 0; i < ancestors.size(); i++)
						{
							string ancestorsLine;
							stringstream gg;
							gg << ancestors.at(i);
							gg >> ancestorsLine;

							Database::insertUses(ancestorsLine, useVariableToken);
						}
					}
					nestedexpressionTokens.pop_front();
				}

			}
			else if (countparent > 0)
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string childLine;
				stringstream cc;
				cc << countlines;
				cc >> childLine;

				Database::insertModifies(parentLine, variableToken);

				while (nestedexpressionTokens.front() != ";")
				{

					if (checkName(nestedexpressionTokens.front())) // if it is a variable
					{
						string useVariableToken = nestedexpressionTokens.front();
						Database::insertUses(parentLine, useVariableToken);
					}
					nestedexpressionTokens.pop_front();
				}
			}

			countlines++;
			currentlines++;
		}

		//if (!nextst.empty())
		//{
		//	cout << "THIS IS THE 2D VECTOR" << endl;
		//	for (int i = 0; i < nextst.size(); i++)
		//	{
		//		for (int j = 0; j < nextst[i].size(); j++)
		//		{
		//			cout << nextst[i].at(j) << ",";
		//		}
		//		cout << endl;
		//	}
		//	cout << endl;
		//}


		//if (!nextstTemp.empty())
		//{
		//	cout << "THIS IS THE TEMP VECTOR" << endl;
		//	for (int i = 0; i < nextstTemp.size(); i++)
		//	{
		//		cout << nextstTemp.at(i) << ",";
		//	}
		//	cout << endl;
		//}



		while (!nextstTemp.empty())
		{

			for (int i = 0; i < nextst.size(); i++) // iterate through the rows of the 2D vector
			{
				if (find(nextst[i].begin(), nextst[i].end(), nextstTemp.front()) != nextst[i].end()) // check if first element in the temp vector is in previous vectors of 2D
				{
					nextst[i].push_back(nextstTemp.at(1));

				}
			}

			for (int k = 0; k < 2; k++)
			{
				nextstInsert.push_back(nextstTemp.at(k));

			}

			nextst.push_back(nextstInsert);
			nextstTemp.erase(nextstTemp.begin(), nextstTemp.begin() + 2);
			nextstInsert.clear();
		}
	}

	next();

	if (!remainingTokens.empty() && remainingTokens.front() == "procedure")
	{
		//cout << nextst[2].at(0) << endl;
		for (int i = 0; i < nextst.size(); i++)
		{
			for (int j = 1; j < nextst[i].size(); j++)
			{
				Database::insertNextst(nextst[i].at(0), nextst[i].at(j)); // insert indirect next into database
				int num2 = stoi(nextst[i].at(j));
				int num1 = stoi(nextst[i].at(j-1));
				if (num2 < num1)
				{
					for (int k = num2; k < num1 + 1 ; k++)
					{
						string insert;
						stringstream ii;
						ii << k;
						ii >> insert;
						Database::insertNextst(nextst[i].at(0), insert);
					}
				}
			}
		}
		//nextst.clear();

		for (int i = 0; i < procedureCalls.size(); i++) // iterating through the rows of the 2D vector
		{
			// check if the current procedure / first element in temp vector is in the previous vectors
			if (find(procedureCalls[i].begin(), procedureCalls[i].end(), procedureList.back()) != procedureCalls[i].end())
			{
				for (int j = 1; j < procedureTemp.size(); j++) // iterate through the temp vector from 2nd element onwards
				{
					procedureCalls[i].push_back(procedureTemp.at(j)); // if it is, include its callee into previous vector

					for (int k = 0; k < procedureCalls.size(); k++)
					{
						if (procedureTemp.at(j) == procedureCalls[k].at(0)) // find if callee is called elsewhere in the 2D vector
						{
							for (int col = 1; col < procedureCalls[k].size(); col++) // if it is, add callee's callee
							{
								procedureCalls[i].push_back(procedureCalls[k].at(col));
								//procedureTemp.push_back(procedureCalls[k].at(col));
							}
						}
					}
				}
			}
		}

		procedureCalls.push_back(procedureTemp); // transfer and store temp vector into vector of vector
		// check if the 2nd element in temp vector onwards is in the previous vector
		for (int j = 1; j < procedureTemp.size(); j++) // iterate through the temp vector from 2nd element onwards
		{
			for (int k = 0; k < procedureCalls.size(); k++) // iterate through the first column of 2D vector
			{
				if (procedureTemp.at(j) == procedureCalls[k].at(0)) // find if callee is called elsewhere in the 2D vector
				{
					for (int col = 1; col < procedureCalls[k].size(); col++) // if it is, add callee's callee
					{
						procedureCalls[procedureList.size()-1].push_back(procedureCalls[k].at(col));
						//procedureTemp.push_back(procedureCalls[k].at(col));
					}
				}
			}
		}

		
		procedureTemp.clear(); // clear away temp vector
		//Database::insertCallst(procedureCalls[0].at(0), procedureCalls[0].at(1)); // insert indirect calls into database
		//Database::insertCallst("test1", "test2");
		parseProcedure();
	}

	if (remainingTokens.empty()) // at the end of the source, extract from procedureCalls vector to determine callst 2d vector
	{
		procedureCalls.push_back(procedureTemp); // transfer and store temp vector into vector of vector
		// check if the 2nd element in temp vector onwards is in the previous vector
		for (int j = 1; j < procedureTemp.size(); j++) // iterate through the temp vector from 2nd element onwards
		{
			for (int k = 0; k < procedureCalls.size(); k++) // iterate through the first column of 2D vector
			{
				if (procedureTemp.at(j) == procedureCalls[k].at(0)) // find if callee is called elsewhere in the 2D vector
				{
					for (int col = 1; col < procedureCalls[k].size(); col++) // if it is, add callee's callee
					{
						procedureCalls[procedureList.size() - 1].push_back(procedureCalls[k].at(col));
						//procedureTemp.push_back(procedureCalls[k].at(col));
					}
				}
			}
		}
		procedureTemp.clear(); // clear away temp vector
		for (int i = 0; i < procedureCalls.size(); i++)
		{
			string caller = procedureCalls[i].at(0);
			for (int j = 1; j < procedureCalls[i].size(); j++)
			{
				Database::insertCallst(caller, procedureCalls[i].at(j)); // insert indirect calls into database
				//Database::insertCallst("test3", "test4");
				//Database::insertCallst(procedureCalls[2].at(0), procedureCalls[2].at(0));
	
				string callee = procedureCalls[i].at(j);
				vector<string> results;

				Database::getCallsTmodifies(results,callee);
				for (int k = 0; k < results.size(); k++)
				{
					Database::insertModifiesproc(caller, results.at(k));
				}

				results.clear();

				Database::getCallsTuses(results, callee);
				for (int k = 0; k < results.size(); k++)
				{
					Database::insertUsesproc(caller, results.at(k));
				}

			}
		}

		for (int i = 0; i < nextst.size(); i++)
		{
			for (int j = 1; j < nextst[i].size(); j++)
			{
				Database::insertNextst(nextst[i].at(0), nextst[i].at(j)); // insert indirect next into database
				int num2 = stoi(nextst[i].at(j));
				int num1 = stoi(nextst[i].at(j - 1));
				if (num2 < num1)
				{
					for (int k = num2; k < num1 + 1; k++)
					{
						string insert;
						stringstream ii;
						ii << k;
						ii >> insert;
						Database::insertNextst(nextst[i].at(0), insert);
					}
				}
			}
		}


		//for (int i = 0; i < procedureCalls.size(); i++) // iterate through the rows of the 2D vector
		//{	
		//	string caller = procedureCalls[i].at(0); // to capture the first item in each row of vector as caller
		//	for (int j = 1; j < procedureCalls[i].size(); j++) // iterate through the columns of the 2D vector, starting from 2nd column
		//	{
		//		string callee = procedureCalls[i].at(j);
		//		for (int k = 1; k < procedureCalls.size(); k++)
		//		{
		//			if (callee == procedureCalls[k].at(0))
		//			{
		//				for (int col = 1; col < procedureCalls[k].size(); col++)
		//				{
		//					Database::insertCallst(caller, procedureCalls[k].at(col));
		//				}


		//				
		//			}
		//		}
		//	}
		//}

	}
}



void SourceProcessor::parseVariable()
{
	if (checkName(remainingTokens.front())) // checks if it is a NAME grammer
	{
		Database::insertVariable(remainingTokens.front()); // insert the variable into the database
		next();
	}
	else
	{
		//std::throw_with_nested("error in parseVariable");
	}
}

void SourceProcessor::parseFactorCondition()
{
	if (checkName(remainingTokens.front())) // if it is a variable
	{
		parseVariable();
	}
	else if (checkInteger(remainingTokens.front())) // if it is an integer
	{
		parseConstant();
	}


	if (match(">") || match("<")) // should be a real expression
	{
		next();
		parseFactorCondition();
	}
	else if (match(")"))
	{
		next();
	}
}

void SourceProcessor::parseFactor()
{
	if (checkName(remainingTokens.front())) // if it is a variable
	{
		parseVariable();
	}
	else if (checkInteger(remainingTokens.front())) // if it is an integer
	{
		parseConstant();
	}

	if (match("+") || match("-") || match("*") || match("/") || match("%") || match("(") || match(")")) // should be an expression sign
	{
		next();
		parseFactor(); // check again if it is an variable or integer or an expression
	}
	else if (match(";"))
	{
		next();
	}
}

void SourceProcessor::parseConstant()
{
	if (checkInteger(remainingTokens.front())) // checks if it is an INTEGER grammer
	{
		Database::insertConstant(remainingTokens.front()); // insert the constant into the database
		next();
	}
	else
	{
		//std::throw_with_nested("error in parseConstant");
	}
}

void SourceProcessor::parseExpression()
{
	//stack<string> operands;
	stack<string> operators;
	vector<string> expression;
	list<string> remainingExprTokens;
	remainingExprTokens = remainingTokens;

	while (remainingExprTokens.front() != ";") {

		//cout << "expression character: " << remainingExprTokens.front();
		//cout << endl;
		//cout << "operands size: " << operators.size();
		//cout << endl;
		//cout << "postfix size: " << expression.size();
		//cout << endl;
		string exprToken = remainingExprTokens.front();

		if (exprToken == "(")
		{
			operators.push(exprToken);
			remainingExprTokens.pop_front();
		}
		else if (exprToken == ")")
		{
			while (operators.top() != "(")
				{
				expression.push_back(operators.top());
				operators.pop();
				//remainingExprTokens.pop_front();
				}
				operators.pop();
				remainingExprTokens.pop_front();
		}
		else if (validateIdent(exprToken) || validateNumber(exprToken)) {
			expression.push_back(exprToken);
			remainingExprTokens.pop_front();
		}
		else if (isOperator(exprToken))
		{
			while (!operators.empty() && precedence(operators.top()) >= precedence(exprToken))
			{
				expression.push_back(operators.top());
				operators.pop();
			}
			operators.push(exprToken);
			remainingExprTokens.pop_front();
		}
	}

	while (!operators.empty())
	{
		expression.push_back(operators.top());
		operators.pop();
	}
	
	for (string i : expression) {
		expr += i;
	}

}

int SourceProcessor::precedence(string symbol) {
	if (symbol == "+" || symbol == "-") {
		return 0;
	}
	else if (symbol == "%" || symbol == "*" || symbol == "/") {
		return 1;
	}
	else {
		return -1;
	}
}

bool SourceProcessor::isOperator(string symbol) {
	if (symbol == "+" || symbol == "-" || symbol == "*" || symbol == "/" || symbol == "%")
	{
		return true;
	}
	else
	{
		false;
	}
}

bool SourceProcessor::validateNumber(string symbol) {
	return regex_match(symbol, regex(INTEGER_PATTERN));
}

bool SourceProcessor::validateIdent(string symbol)
{
	return regex_match(symbol, regex(IDENT_PATTERN));
}