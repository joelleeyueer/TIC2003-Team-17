#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "UsesPClauseEvaluator.h"
#include "Query.h"


using namespace std;

void UsesPClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getUsesP(results, parentType, parentValue, childType, childValue);
}
