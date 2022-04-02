#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "NextTClauseEvaluator.h"
#include "Query.h"


using namespace std;

void NextTClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getNextT(results, parentType, parentValue, childType, childValue);
}
