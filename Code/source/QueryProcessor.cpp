#include "QueryProcessor.h"
#include "SelectClauseEvaluator.h"
#include "PatternClauseEvaluator.h"
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
		// if rel ref == parenmt
		// else if rel ref = modifies.. etc etc
	}
	else if (queryObj.patternClauses.size() > 0) {
		// evaluate pattern clause
		vector<vector<string>> patternClauseResults;
		evaluatePatternClause(queryObj, patternClauseResults);
		//filter 
		//assign a, b; select a pattern a (a, _"b"_) ==/==
		//assign a, b; select b pattern a (a, _"b"_) 
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
	//
}


