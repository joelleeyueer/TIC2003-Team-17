#pragma once
#include <string>
#include <vector>

using namespace std;

typedef struct {
    string name;
    string designEntity;
} SelectClause;

typedef struct {
    string relRef; //e.g. uses, modifies, parent
    vector<string> firstArgument; //index 0 is design entity index 1 is synonym, i.e. uses(a,v) = firstArgument[0] = a, firstArgument[1] = v
    vector<string> secondArgument;
} SuchThatClause;

class Query
{
public:
    Query();

    void addSelectClause(string name, string designEntity);
    void addSuchThatClause(string relRef, vector<string> firstArgument, vector<string> secondArgument);

    vector<SelectClause> selectClauses;
    vector<SuchThatClause> suchThatClauses;
};

