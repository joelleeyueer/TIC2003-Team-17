#pragma once
#include <sstream>
#include <fstream>
#include <vector>
#include "Database.h"
#include "Tokenizer.h"
#include <list>
#include <stdexcept>


using namespace std;

class SourceProcessor {
public:
	// method for processing the source program
	list<string> remainingTokens;
	string expr;

	void process(string program);
	void parse(list<string> tokens);
	void parseProgram();
	bool checkName(string token);
	bool checkInteger(string token);
	void parseProcedure();
	void parseStatement();
	void parseVariable();
	void parseFactorCondition();
	void parseFactor();
	void parseConstant();
	void parseExpression();
	bool validateNumber(string symbol);
	bool validateIdent(string symbol);
	bool isOperator(string symbol);

private:
	void expect(string symbol);
	bool match(string symbol);
	void next();
	int precedence(string symbol);
};