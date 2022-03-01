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
			//Database::getUses(output, firstArg_index0, firstArg_index1, secondArg_index0, secondArg_index1)
		}

		//filter (need help)

		if (suchThatResults.empty()) {
			return;
		}

		if (suchThatResults[0][0] != selectClauseResults[0] && suchThatResults[1][0] != selectClauseResults[0]) {
			return selectClauseResults[0];
		}
		
		/*
		* 1. check if such that clause (i.e. pattern, etc) returns an empty vector of vectors. if yes, just return an empty vector as the output
		* 
		* 2. check if Select clause synonym is the same as such that first argument OR second argument in the such that clause
		* 2. if not the same, then just return the answer of the Select clause
		* 3. if it's the same, then check which argument (i.e. first or second) has the same synonym.
		* 4. for e.g. assign a; variable v; Select v such that Modifies(a, v) -> v is the smilar synonym AND it si in the seconfd argument => LOOK AT THE second index of each element in the result vector
		*    if result from Modifies clause => { { 1, "v" }, { 2, "p" } } AND THE SIMILAR SYNONYM IS the second argument. then you literally just extract the index 1 into another vector and return that
		*	-> { "v", "p" }
		*/	
	}
	else if (queryObj.patternClauses.size() > 0) {
		// evaluate pattern clause
		vector<vector<string>> patternClauseResults;
		evaluatePatternClause(queryObj, patternClauseResults);
		

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


