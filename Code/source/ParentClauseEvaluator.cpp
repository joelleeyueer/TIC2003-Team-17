#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "ParentClauseEvaluator.h"
#include "Query.h"


using namespace std;

void ParentClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getParent(results, parentType, parentValue, childType, childValue);
}
