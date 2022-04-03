#include <iostream>
#include <algorithm>
#include "Database.h"
#include "SelectClauseEvaluator.h"

using namespace std;

void SelectClauseEvaluator::evaluate(vector<string>& results, string designEntityString)
{
	vector<string> tempDatabaseResult;
	if (designEntityString == "procedure") {
		Database::getProcedure(tempDatabaseResult);
	}

	else if (designEntityString == "constant") {
		Database::getConstant(tempDatabaseResult);
	}

	else if (designEntityString == "variable") {
		Database::getVariable(tempDatabaseResult);
	}

	else if (designEntityString == "stmt") {
		Database::getStatement(tempDatabaseResult);
	}

	else if (designEntityString == "read") {
		Database::getRead(tempDatabaseResult);
	}

	else if (designEntityString == "print") {
		Database::getPrint(tempDatabaseResult);
	}

	else if (designEntityString == "assign") {
		Database::getAssignment(tempDatabaseResult);
	}

	else if (designEntityString == "while") {
		Database::getWhile(tempDatabaseResult);
	}

	else if (designEntityString == "if") {
		Database::getIf(tempDatabaseResult);
	}

	//remove duplicates from vector before pushing it into databaseResult string
	removeDupe(tempDatabaseResult);

	// post process the results to fill in the output vector
	for (string value : tempDatabaseResult) {
		results.push_back(value);
	}
}

void SelectClauseEvaluator::removeDupe(vector<string>& results)
{
	sort(results.begin(), results.end());
	results.erase(unique(results.begin(), results.end()), results.end());
}
