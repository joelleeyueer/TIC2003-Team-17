#pragma once
#include <sstream>
#include <fstream>
#include<list>
#include <map>
#include <stdexcept>

#include "Query.h"

using namespace std;

class QueryParser
{
public:
	list<string> remainingTokens;
	map<string, string> currentDeclarationList;
	vector<string> expression;

	void parse(list<string> tokens, Query& query);

	void parseDeclarationList(Query& currentQuery);
	void parseSelectClause(Query& currentQuery);
	void parseSuchThatClause(Query& currentQuery);
	void parsePatternClause(Query& currentQuery);
	void parseExpression(Query& currentQuery);
	bool validateSynonym(string symbol, bool checkInDeclaration);

	void validateDesignEntity(string symbol);
	bool validateIdent(string symbol);
	bool validateNumber(string symbol);

private:
	int precedence(string symbol);
	void expect(string symbol);
	bool match(string symbol);
	void next();
};

