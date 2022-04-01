#include "QueryProcessor.h"
#include "SelectClauseEvaluator.h"
#include "PatternClauseEvaluator.h"
#include "ParentClauseEvaluator.h"
#include "ParentTClauseEvaluator.h"
#include "ModifiesClauseEvaluator.h"
#include "UsesClauseEvaluator.h"
#include "Tokenizer.h"
#include "Database.h"
#include "QueryTable.cpp"
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

	QueryTable qt;
	vector<string> selectClauseResults;
	vector<vector<string>> suchThatResults;
	vector<vector<string>> patternClauseResults;

	evaluateSelectClause(queryObj, selectClauseResults);
	evaluateSuchThatClause(queryObj, suchThatResults);
	//evaluatePatternClause(queryObj, patternClauseResults);

	//TBA drop columns operation
	qt.dropColumns(queryObj);
	qt.queryToOutput(output);
	
}

void QueryProcessor::evaluateSuchThatClause(Query queryObj, vector<vector<string>>& suchThatResults)
{
	/// <summary>
	/// Iterate through suchThatResults and parse results into suchThatMaster
	/// </summary>
	
	QueryTable qt;

	int suchThatCount = queryObj.suchThatClauses.size();
	int iterator = 0;

	while (suchThatCount > 0 && iterator > suchThatCount) {
		
		// if rel ref == parent
		// else if rel ref = modifies.. etc etc
		string relRef = queryObj.suchThatClauses[iterator].relRef;


		if (relRef == "Parent") {
			evaluateParentClause(queryObj, suchThatResults, iterator);
		}
		else if (relRef == "Parent*") {
			evaluateParentTClause(queryObj, suchThatResults, iterator);
		}
		else if (relRef == "Modifies") {
			evaluateModifiesClause(queryObj, suchThatResults, iterator);
		}
		else if (relRef == "Uses") { //relRef == "Uses
			evaluateUsesClause(queryObj, suchThatResults, iterator);
		}
		else {
			return;
		}

		qt.evaluateIncomingSuchThat(queryObj, suchThatResults, iterator);
		suchThatResults.clear();
		iterator++;

	}
}

void QueryProcessor::evaluatePatternClause(Query queryObj, vector<vector<string>>& patternClauseResults)
{
	/// <summary>
	/// Iterate through patternClauseResults and parse results into patternClauseMaster
	/// </summary>

	
}





void QueryProcessor::filterSuchThatPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output){
	//suchThat + Pattern, but Pattern is meaningless
	//Select synonym == suchThat first Arg or second Arg and Select synonym != pattern synonym or pattern LHS
	if ((queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].firstArgument[1] || queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].secondArgument[1]) && !(queryObj.selectClauses[0].name == queryObj.patternClauses[0].patternSynonym || queryObj.selectClauses[0].name == queryObj.patternClauses[0].LHS[1])) {
		filterOnlySuchThatClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
	}
	
	//suchThat + Pattern, but Such That is meaningless
	//Select synonym != suchThat first Arg or second Arg and Select synonym == pattern synonym or pattern LHS
	else if ((queryObj.selectClauses[0].name == queryObj.patternClauses[0].patternSynonym || queryObj.selectClauses[0].name == queryObj.patternClauses[0].LHS[1]) && !(queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].firstArgument[1] || queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].secondArgument[1])) {
		filterOnlyPatternClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
	}

	//Such That and Pattern are not meaningless
	
	else { 
		//Scenario A: I have a Parent/Parent* relRef,
		//hence such That second argument synonym == assignment
		//pattern synonym == assignment ONLY
		if ((queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].secondArgument[1] && queryObj.selectClauses[0].name == queryObj.patternClauses[0].patternSynonym) && queryObj.selectClauses[0].designEntity == "assign") {			
			if (queryObj.suchThatClauses[0].relRef == "Modifies" || queryObj.suchThatClauses[0].relRef == "Uses") {
				for (vector<string> STrow : suchThatResults) {
					for (vector<string> PTrow : patternClauseResults) {
						if (STrow[0] == PTrow[0]) {
							output.push_back(STrow[0]);
						}
					}
				}
			}
			else {
				for (vector<string> STrow : suchThatResults) {
					for (vector<string> PTrow : patternClauseResults) {
						if (STrow[1] == PTrow[0]) {
							output.push_back(STrow[1]);
						}
					}
				}
			}
		}
		
		//Scenario B: I have a Modifies/Uses relRef
		//hence such That second argument synonym == variable
		//pattern first argument == variable ONLY
		else if ((queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].secondArgument[1] && queryObj.selectClauses[0].name == queryObj.patternClauses[0].LHS[1]) && queryObj.selectClauses[0].designEntity == "variable") {
			for (vector<string> STrow : suchThatResults) {
				for (vector<string> PTrow : patternClauseResults) {
					if (STrow[1] == PTrow[1]) {
						output.push_back(STrow[1]);
					}
				}
			}
		}

		else if ((queryObj.selectClauses[0].name == queryObj.suchThatClauses[0].firstArgument[1] && queryObj.selectClauses[0].name == queryObj.patternClauses[0].patternSynonym) && queryObj.selectClauses[0].designEntity == "assign") {
			if (queryObj.suchThatClauses[0].secondArgument[1] == queryObj.patternClauses[0].LHS[1]) {
				for (vector<string> STrow : suchThatResults) {
					for (vector<string> PTrow : patternClauseResults) {
						if (STrow[0] == PTrow[0] && STrow[1] == PTrow[1]) {
							output.push_back(STrow[0]);
						}
					}
				}
			}
			else {
				for (vector<string> STrow : suchThatResults) {
					for (vector<string> PTrow : patternClauseResults) {
						if (STrow[0] == PTrow[0]) {
							output.push_back(STrow[0]);
						}
					}
				}
			}
		}

		else {
			return;
		}
	}
}

void QueryProcessor::filterOnlySuchThatClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output)
{
	// only such that exists
	if (!(suchThatResults.empty()) && (patternClauseResults.empty() && queryObj.patternClauses.empty())) { // Pattern Clause is empty

		//meaningless query: both first Argument synonym in suchThat != select Clause and second Argument synonym in suchThat != select Clause)
		if ((queryObj.suchThatClauses[0].firstArgument[1] != queryObj.selectClauses[0].name) && (queryObj.suchThatClauses[0].secondArgument[1] != queryObj.selectClauses[0].name)) {
			for (string row : selectClauseResults) {
				output.push_back(row);
			}
		}
		//not meaningless
		//Select a such that Modifies(_,a)
		else if (queryObj.suchThatClauses[0].firstArgument[1] == queryObj.selectClauses[0].name) {
			for (vector<string> row : suchThatResults) {
				output.push_back(row[0]);
			}

		}
		//Select a such that Modifies(a,_)
		else if (queryObj.suchThatClauses[0].secondArgument[1] == queryObj.selectClauses[0].name) {

			// if the select wants a statement number and the rel ref is a modifies or uses, we shuld take the first column for the such that clause (bc thats where the stmt nunber is)
			if ((queryObj.selectClauses[0].designEntity != "variable") && ((queryObj.suchThatClauses[0].relRef == "Modifies") || (queryObj.suchThatClauses[0].relRef == "Uses"))) {
				for (vector<string> row : suchThatResults) {
					output.push_back(row[0]);
				}
			}
			else {
				for (vector<string> row : suchThatResults) {
					output.push_back(row[1]);
				}
			}
		}
	}
}

void QueryProcessor::filterOnlyPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output)
{
	// only pattern exists
	if ((suchThatResults.empty() && queryObj.suchThatClauses.empty()) && !(patternClauseResults.empty())) { // Such That Clause is empty

		//meaningless query
		if (queryObj.patternClauses[0].patternSynonym != queryObj.selectClauses[0].name) {
			for (string row : selectClauseResults) {
				output.push_back(row);
			}
		}
		else if (queryObj.patternClauses[0].patternSynonym == queryObj.selectClauses[0].name) {
			for (vector<string> row : patternClauseResults) {
				output.push_back(row[0]);
			}
		}
	}
}

void QueryProcessor::evaluateSelectClause(Query query, vector<string>& results)
{
	SelectClauseEvaluator selectClauseEvaluator;
	int selectCount = query.selectClauses.size();
	int iterator = 0;

	while (selectCount > 0 && iterator > selectCount) {
		selectClauseEvaluator.evaluate(results, query.selectClauses[iterator].designEntity);
		iterator++;
	}
}

void QueryProcessor::evaluatePatternClause(Query query, vector<vector<string>>& results, int iterator)
{
	PatternClauseEvaluator patternClauseEvaluator;
	patternClauseEvaluator.evaluate(results, query.patternClauses[iterator].LHS, query.patternClauses[iterator].RHS, query.declarationList);
}

void QueryProcessor::evaluateParentClause(Query query, vector<vector<string>>& results, int iterator)
{
	ParentClauseEvaluator parentClauseEvaluator;
	parentClauseEvaluator.evaluate(results, query.suchThatClauses[iterator].firstArgument[0], query.suchThatClauses[iterator].firstArgument[1], query.suchThatClauses[iterator].secondArgument[0], query.suchThatClauses[iterator].secondArgument[1]);
}

void QueryProcessor::evaluateParentTClause(Query query, vector<vector<string>>& results, int iterator)
{
	ParentTClauseEvaluator parentTClauseEvaluator;
	parentTClauseEvaluator.evaluate(results, query.suchThatClauses[iterator].firstArgument[0], query.suchThatClauses[iterator].firstArgument[1], query.suchThatClauses[iterator].secondArgument[0], query.suchThatClauses[iterator].secondArgument[1]);

}

void QueryProcessor::evaluateModifiesClause(Query query, vector<vector<string>>& results, int iterator)
{
	ModifiesClauseEvaluator modifiesClauseEvaluator;
	modifiesClauseEvaluator.evaluate(results, query.suchThatClauses[iterator].firstArgument[0], query.suchThatClauses[iterator].firstArgument[1], query.suchThatClauses[iterator].secondArgument[0], query.suchThatClauses[iterator].secondArgument[1]);
}

void QueryProcessor::evaluateUsesClause(Query query, vector<vector<string>>& results, int iterator)
{
	UsesClauseEvaluator usesClauseEvaluator;
	usesClauseEvaluator.evaluate(results, query.suchThatClauses[iterator].firstArgument[0], query.suchThatClauses[iterator].firstArgument[1], query.suchThatClauses[iterator].secondArgument[0], query.suchThatClauses[iterator].secondArgument[1]);
}






