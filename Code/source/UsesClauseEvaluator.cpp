#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "Query.h"
#include "UsesClauseEvaluator.h"


using namespace std;


void UsesClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getUses(results, parentType, parentValue, childType, childValue);
}
