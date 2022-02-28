#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "ModifiesClauseEvaluator.h"
#include "Query.h"


using namespace std;

void ModifiesClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getModifies(results, parentType, parentValue, childType, childValue);
}
