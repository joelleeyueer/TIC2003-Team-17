#pragma once
#include <string>
#include <vector>

using namespace std;

typedef struct {
    std::string name;
    std::string designEntity;
} SelectClause;

class Query
{
public:
    Query();

    void addSelectClause(string name, string designEntity);

    vector<SelectClause> selectClauses;
};

