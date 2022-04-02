#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "NextClauseEvaluator.h"
#include "Query.h"


using namespace std;

void NextClauseEvaluator::evaluate(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	Database::getNext(results, parentType, parentValue, childType, childValue);
}
