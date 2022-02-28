#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "ParentTClauseEvaluator.h"
#include "Query.h"


using namespace std;

void ParentTClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getParentT(results, parentType, parentValue, childType, childValue);
}
