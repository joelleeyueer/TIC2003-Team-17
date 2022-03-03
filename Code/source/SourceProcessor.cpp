#include "SourceProcessor.h"

#include <regex>
#include <iostream>

#define INTEGER_PATTERN "[0-9]+"
#define NAME_PATTERN "[a-zA-Z][a-zA-Z0-9]*"

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
		std::throw_with_nested("error in parsing query");
	}
}

void SourceProcessor::expect(string symbol)
{
	if (match(symbol)) {
		next();
		return;
	}

	std::throw_with_nested("expected: '" + symbol + "', got '" + remainingTokens.front() + "' instead");
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
		std::throw_with_nested("error in declaration list");
	}
}

void SourceProcessor::parseProcedure()
{
	if (remainingTokens.front() == "procedure")
	{
		next();
		if (checkName(remainingTokens.front())) // checks if it is a NAME grammer
		{
			Database::insertProcedure(remainingTokens.front()); // insert the procedure into the database
			next();
			expect("{");
			parseStatement();
		}
		else
		{
			std::throw_with_nested("error in parseProcedure");
		}
	}
}

int countlines = 1;
int countparent = 0;
int parentChild = 0; // it is not a child, = 1 if it is a child
int prevCountparent = 0;
string lhs;
string rhs;

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

			string grandparentLine;
			stringstream gg;
			gg << parentChild;
			gg >> grandparentLine;

			Database::insertChild(parentLine, childLine); // parent table
			Database::insertGrandchild(grandparentLine, childLine); // parent* table
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

			Database::insertChild(parentLine, childLine); // parent table
			//Database::insertGrandchild(parentLine, childLine); // parent* table
		}

		string statementLine;
		stringstream ss;
		ss << countlines;
		ss >> statementLine;
		Database::insertStatement(statementLine);

		if (remainingTokens.front() == "while")
		{
			parentChild = countparent;
			prevCountparent = countparent;
			countparent = countlines;
			string whileLine;
			stringstream ss;
			ss << countlines;
			ss >> whileLine;
			Database::insertWhile(whileLine);

			next();
			expect("(");
			list<string> conditionTokens = remainingTokens;
			parseFactor();
			expect("{");
			countlines++;
			parseStatement();

			//if (remainingTokens.front() == "}")
			//{
			countparent = 0;
			parentChild = 0;
			//}
			//else
			//{
			//	countparent = prevCountparent;
			//}

			while (conditionTokens.front() != ")")
			{
				if (checkName(conditionTokens.front())) // if it is a variable
				{
					string useVariableToken = conditionTokens.front();
					Database::insertUses(whileLine, useVariableToken);
				}
				conditionTokens.pop_front();
			}

		}
		else if (remainingTokens.front() == "if")
		{
			parentChild = countparent;
			prevCountparent = countparent;
			countparent = countlines;
			string ifLine;
			stringstream ss;
			ss << countlines;
			ss >> ifLine;
			Database::insertIf(ifLine);

			next();
			expect("(");
			list<string> conditionTokens = remainingTokens;
			parseFactor();
			expect("then");
			expect("{");
			countlines++;
			parseStatement();

			if (remainingTokens.front() == "else")
			{
				next();
				expect("{");
				parseStatement();
			}

			//if (remainingTokens.front() == "}")
			//{
			countparent = 0;
			parentChild = 0;
			//}
			//else
			//{
			//	countparent = prevCountparent;
			//}

			while (conditionTokens.front() != ")")
			{
				if (checkName(conditionTokens.front())) // if it is a variable
				{
					string useVariableToken = conditionTokens.front();
					Database::insertUses(ifLine, useVariableToken);
				}
				conditionTokens.pop_front();
			}

		}
		else if (remainingTokens.front() == "read")
		{

			string readLine;
			stringstream ss;
			ss << countlines;
			ss >> readLine;

			string modifyLine;
			stringstream mm;
			mm << countlines;
			mm >> modifyLine;

			Database::insertRead(readLine);
			countlines++;

			next();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect(";");
			Database::insertModifies(modifyLine, variableToken);

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string grandparentLine;
				stringstream gg;
				gg << parentChild;
				gg >> grandparentLine;

				Database::insertModifies(parentLine, variableToken); // 
				Database::insertModifies(grandparentLine, variableToken); // 
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


		}
		else if (remainingTokens.front() == "print")
		{
			string printLine;
			stringstream ss;
			ss << countlines;
			ss >> printLine;

			string useLine;
			stringstream uu;
			uu << countlines;
			uu >> useLine;

			Database::insertPrint(printLine);
			countlines++;

			next();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect(";");
			Database::insertUses(useLine, variableToken);

			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string grandparentLine;
				stringstream gg;
				gg << parentChild;
				gg >> grandparentLine;

				Database::insertUses(parentLine, variableToken); // 
				Database::insertUses(grandparentLine, variableToken); // 
			}

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

			countlines++;

			lhs = remainingTokens.front();
			string variableToken = remainingTokens.front();
			parseVariable();
			expect("=");
			list<string> expressionTokens = remainingTokens;
			parseFactor(); // factor can be either a variableName, Constant, Expre

			while (expressionTokens.front() != ";")
			{
				rhs += expressionTokens.front();
				if (checkName(expressionTokens.front())) // if it is a variable
				{
					string useVariableToken = expressionTokens.front();
					Database::insertUses(useLine, useVariableToken);
				}
				expressionTokens.pop_front();
			}

			Database::insertAssignment(assignmentLine, lhs, rhs);
			Database::insertModifies(modifyLine, variableToken);
			if (countparent > 0 && parentChild > 0) // 5(sub parent) and 3(bigger parent)
			{
				string parentLine;
				stringstream pp;
				pp << countparent;
				pp >> parentLine;

				string grandparentLine;
				stringstream gg;
				gg << parentChild;
				gg >> grandparentLine;

				Database::insertModifies(parentLine, variableToken);
				Database::insertModifies(grandparentLine, variableToken);
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
			}
			//expect(";");
		}
	}

	next();
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
		std::throw_with_nested("error in parseVariable");
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

	if (!match(";")) // this means it is an expression or real expression
	{
		if (match("+") || match("-") || match("*") || match("/") || match("%")) // should be an expression sign
		{
			next();
			parseFactor(); // check again if it is an variable or integer or an expression
		}
		else if (match(">") || match("<")) // should be a real expression
		{
			next();
			parseFactor();
		}
		else if (match(")"))
		{
			next();
		}
		else
		{
			// throw error
		}
	}
	else
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
		std::throw_with_nested("error in parseConstant");
	}
}