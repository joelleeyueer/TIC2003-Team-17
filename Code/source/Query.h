#pragma once
#include <string>
#include <vector>
#include <map>

#include "Clause.h"

using namespace std;

struct SelectClause {
    string name;
    string designEntity;
};

struct SuchThatClause : Clause {
    string relRef; //e.g. uses, modifies, parent
    vector<string> firstArgument; //index 0 is design entity / undeclared / line number or index 1 is synonym, i.e. uses(a,v) = firstArgument[0] = a, firstArgument[1] = v
    vector<string> secondArgument;
    ClauseType getType() override { return CLAUSE_SUCH_THAT; }
};

struct PatternClause : Clause {
    string patternSynonym;
    vector<string> LHS; //index 0 is design entity/undeclared/IDENT, index 1 is entity Reference (synonym, _ or "IDENT")
    vector<string> RHS; //index 0 is partial match/exact match/undeclared, index 1 is the expression
    ClauseType getType() override { return CLAUSE_PATTERN; }
};

class Query
{
public:
    Query();

    void addSelectClause(string name, string designEntity);
    void addSuchThatClause(string relRef, vector<string> firstArgument, vector<string> secondArgument);
    void addPatternClause(string synonymName, vector<string> LHS, vector<string> RHS);
    void addDeclarationList(map<string, string> declarationList);

    vector<SelectClause> selectClauses;
    vector<SuchThatClause> suchThatClauses;
    vector<PatternClause> patternClauses;
    map<string, string> declarationList;
};

