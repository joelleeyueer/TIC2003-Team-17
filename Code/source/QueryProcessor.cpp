#include "QueryProcessor.h"
#include "Tokenizer.h"
#include "Database.h"
#include <iostream>
#include <algorithm> 

// constructor
QueryProcessor::QueryProcessor() {}

// destructor
QueryProcessor::~QueryProcessor() {}

// method to evaluate a query
// This method currently only handles queries for getting all the procedure names,
// using some highly simplified logic.
// You should modify this method to complete the logic for handling all required queries.


void QueryProcessor::evaluate(Query queryObj, vector<string>& output) {
	// clear the output vector
	output.clear();

	// check what type of synonym is being declared
	/*string synonymType = tokens.at(0);*/

	// create a vector for storing the results from database
	vector<string> tempDatabaseResult;
	

	// call the method in database to retrieve the results
	// This logic is highly simplified based on iteration 1 requirements and 
	// the assumption that the queries are valid.
	string designEntityString = queryObj.selectClauses[0].designEntity;
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


	//remove duplicates from vector before pushing it into databaseResult string
	removeDupe(tempDatabaseResult);

	// post process the results to fill in the output vector
	for (string databaseResult : tempDatabaseResult) {
		output.push_back(databaseResult);
	}

}

void QueryProcessor::removeDupe(vector<string>& results)
{
	sort(results.begin(), results.end());
	results.erase(unique(results.begin(), results.end()), results.end());
}
