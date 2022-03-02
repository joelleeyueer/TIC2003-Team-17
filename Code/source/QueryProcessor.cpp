#include "QueryProcessor.h"
#include "SelectClauseEvaluator.h"
#include "PatternClauseEvaluator.h"
#include "ParentClauseEvaluator.h"
#include "ParentTClauseEvaluator.h"
#include "ModifiesClauseEvaluator.h"
#include "UsesClauseEvaluator.h"
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

	vector<string> selectClauseResults;
	evaluateSelectClause(queryObj, selectClauseResults);

	if (queryObj.suchThatClauses.size() > 0) {
		// if rel ref == parent
		// else if rel ref = modifies.. etc etc
		string relRef = queryObj.suchThatClauses[0].relRef;

		vector<vector<string>> suchThatResults;

		if (relRef == "Parent") {
			evaluateParentClause(queryObj, suchThatResults);
		}
		else if (relRef == "Parent*") {
			evaluateParentTClause(queryObj, suchThatResults);
		}
		else if (relRef == "Modifies") {
			evaluateModifiesClause(queryObj, suchThatResults);
		}
		else { //relRef == "Uses
			evaluateUsesClause(queryObj, suchThatResults);
		}


		if (suchThatResults.empty()) {
			return;
		}

		if ((queryObj.suchThatClauses[0].firstArgument[1] != queryObj.selectClauses[0].name) && (queryObj.suchThatClauses[0].secondArgument[1] != queryObj.selectClauses[0].name)) {
			for (string row : selectClauseResults) {
				output.push_back(row);
			}
		}
		else if (queryObj.suchThatClauses[0].firstArgument[1] != queryObj.selectClauses[0].name) {
			for (vector<string> row : suchThatResults) {
				output.push_back(row[0]); // you only want the first column of the results because the synonym is common on the first argument
			}
		}
		else if (queryObj.suchThatClauses[0].secondArgument[1] != queryObj.selectClauses[0].name) {
			for (vector<string> row : suchThatResults) {
				output.push_back(row[1]); // you only want the second column of the results because the synonym is common on the secon argument
			}
		}
	}
	else if (queryObj.patternClauses.size() > 0) {
		vector<vector<string>> patternClauseResults;
		evaluatePatternClause(queryObj, patternClauseResults);

		if (patternClauseResults.empty()) {
			return;
		}

		if (queryObj.patternClauses[0].patternSynonym != queryObj.selectClauses[0].name) {
			for (string row : selectClauseResults) {
				output.push_back(row);
			}
		}	
		else if (queryObj.patternClauses[0].patternSynonym != queryObj.selectClauses[0].name) {
			for (vector<string> row : patternClauseResults) {
				output.push_back(row[0]);
			}
		}
	}
	else {
		for (string row : selectClauseResults) {
			output.push_back(row);
		}
	}
}

void QueryProcessor::evaluateSelectClause(Query query, vector<string>& results)
{
	SelectClauseEvaluator selectClauseEvaluator;
	selectClauseEvaluator.evaluate(results, query.selectClauses[0].designEntity); 
}

void QueryProcessor::evaluatePatternClause(Query query, vector<vector<string>>& results)
{
	PatternClauseEvaluator patternClauseEvaluator;
	patternClauseEvaluator.evaluate(results, query.patternClauses[0].LHS, query.patternClauses[0].RHS, query.declarationList);
}

void QueryProcessor::evaluateParentClause(Query query, vector<vector<string>>& results)
{
	ParentClauseEvaluator parentClauseEvaluator;
	parentClauseEvaluator.evaluate(results, query.suchThatClauses[0].firstArgument[0], query.suchThatClauses[0].firstArgument[1], query.suchThatClauses[0].secondArgument[0], query.suchThatClauses[0].secondArgument[1]);
}

void QueryProcessor::evaluateParentTClause(Query query, vector<vector<string>>& results)
{
	ParentTClauseEvaluator parentTClauseEvaluator;
	parentTClauseEvaluator.evaluate(results, query.suchThatClauses[0].firstArgument[0], query.suchThatClauses[0].firstArgument[1], query.suchThatClauses[0].secondArgument[0], query.suchThatClauses[0].secondArgument[1]);

}

void QueryProcessor::evaluateModifiesClause(Query query, vector<vector<string>>& results)
{
	ModifiesClauseEvaluator modifiesClauseEvaluator;
	modifiesClauseEvaluator.evaluate(results, query.suchThatClauses[0].firstArgument[0], query.suchThatClauses[0].firstArgument[1], query.suchThatClauses[0].secondArgument[0], query.suchThatClauses[0].secondArgument[1]);
}

void QueryProcessor::evaluateUsesClause(Query query, vector<vector<string>>& results)
{
	UsesClauseEvaluator usesClauseEvaluator;
	usesClauseEvaluator.evaluate(results, query.suchThatClauses[0].firstArgument[0], query.suchThatClauses[0].firstArgument[1], query.suchThatClauses[0].secondArgument[0], query.suchThatClauses[0].secondArgument[1]);
}


