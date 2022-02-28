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


void QueryProcessor::evaluate(Query queryObj, vector<vector<string>>& output) {
	// clear the output vector
	output.clear();

	vector<string> selectClauseResults;
	evaluateSelectClause(queryObj, selectClauseResults);

	if (queryObj.suchThatClauses.size() > 0) {
		// if rel ref == parenmt
		// else if rel ref = modifies.. etc etc
		string relRef = queryObj.suchThatClauses[0].relRef;
		string firstArg_index0 = queryObj.suchThatClauses[0].firstArgument[0];
		string firstArg_index1 = queryObj.suchThatClauses[0].firstArgument[1];
		string secondArg_index0 = queryObj.suchThatClauses[0].secondArgument[0];
		string secondArg_index1 = queryObj.suchThatClauses[0].secondArgument[1];

		if (relRef == "Parent") {
			vector<vector<string>> parentClauseResults;
			evaluateParentClause(queryObj, parentClauseResults);
		}
		else if (relRef == "Parent*") {
			vector<vector<string>> parentTClauseResults;
			evaluateParentTClause(queryObj, parentTClauseResults);
		}
		else if (relRef == "Modifies") {
			vector<vector<string>> ModifiesClauseResults;
			evaluateModifiesClause(queryObj, ModifiesClauseResults);
		}
		else { //relRef == "Uses
			//Database::getUses(output, firstArg_index0, firstArg_index1, secondArg_index0, secondArg_index1)
		}
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


