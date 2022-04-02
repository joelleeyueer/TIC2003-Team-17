#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"
#include "QueryProcessor.h"

using namespace std;


class SelectClauseEvaluator
{
public:
    void evaluate(vector<vector<string>>& results, string designEntityString);

private:
    void removeDupe(vector<string>& results);
};

