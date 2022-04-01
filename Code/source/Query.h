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

struct SuchThatClause {
    string relRef; //e.g. uses, modifies, parent
    vector<string> firstArgument; //index 0 is design entity / undeclared / line number or index 1 is synonym, i.e. uses(a,v) = firstArgument[0] = a, firstArgument[1] = v
    vector<string> secondArgument;
};

struct PatternClause {
    string patternSynonym;
    vector<string> LHS; //index 0 is design entity/undeclared/IDENT, index 1 is entity Reference (synonym, _ or "IDENT")
    vector<vector<string>> RHS; //vector 0 is partial match/exact match/undeclared, vector 1 is the expression
    
};

class Query
{
public:
    Query();

    void addSelectClause(string name, string designEntity);
    void addSuchThatClause(string relRef, vector<string> firstArgument, vector<string> secondArgument);
    void addPatternClause(string synonymName, vector<string> LHS, vector<vector<string>> RHS);
    void addDeclarationList(map<string, string> declarationList);

    vector<SelectClause> selectClauses;
    vector<SuchThatClause> suchThatClauses;
    vector<PatternClause> patternClauses;
    map<string, string> declarationList;
};

