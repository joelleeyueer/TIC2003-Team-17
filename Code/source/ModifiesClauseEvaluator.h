#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"
#include "QueryProcessor.h"

using namespace std;


class ModifiesClauseEvaluator
{
public:
    void evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue);
};

