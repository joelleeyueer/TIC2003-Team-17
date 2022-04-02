#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "CallsClauseEvaluator.h"
#include "Query.h"


using namespace std;

void CallsClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getCalls(results, parentType, parentValue, childType, childValue);
}
