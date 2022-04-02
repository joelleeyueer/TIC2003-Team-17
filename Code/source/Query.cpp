#include "Query.h"
#include <map>

Query::Query()
{
	selectClauses = vector<SelectClause>{};
	suchThatClauses = vector<SuchThatClause>{};
	patternClauses = vector<PatternClause>{};
}

void Query::addSelectClause(string name, string designEntity)
{
	selectClauses.push_back(SelectClause{ name, designEntity });
}

void Query::addSuchThatClause(string relRef, vector<string> firstArgument, vector<string> secondArgument)
{
	suchThatClauses.push_back(SuchThatClause{ relRef, firstArgument, secondArgument });
}

void Query::addPatternClause(string relRef, vector<string> LHS, vector<string> RHS)
{
	patternClauses.push_back(PatternClause{ relRef, LHS, RHS });
}

void Query::addDeclarationList(map<string, string> declarationList)
{
	this->declarationList = declarationList;
}
