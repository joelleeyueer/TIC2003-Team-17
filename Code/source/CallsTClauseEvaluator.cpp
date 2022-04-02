#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "CallsTClauseEvaluator.h"
#include "Query.h"


using namespace std;

void CallsTClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getCallsT(results, parentType, parentValue, childType, childValue);
}
