#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "ModifiesPClauseEvaluator.h"
#include "Query.h"


using namespace std;

void ModifiesPClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getModifiesP(results, parentType, parentValue, childType, childValue);
}
