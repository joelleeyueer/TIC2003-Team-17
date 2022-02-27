#pragma once
#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"
#include "QueryProcessor.h"

using namespace std;


class PatternClauseEvaluator
{
public:
    void evaluate(vector<vector<string>>& results, vector<string> LHSPair, vector<string> RHSPair, map<string, string> declarationList);
    void evaluateLHSUnrestricted(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, vector<string> RHSPair, map<string, string> declarationList);
    void evaluateLHSIdent(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, string LHSidentText, vector<string> RHSPair, map<string, string> declarationList);
    void evaluateLHSSynonym(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, vector<string> RHSPair, map<string, string> declarationList);

private:
};

