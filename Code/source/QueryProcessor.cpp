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
	vector<vector<string>> suchThatResults;
	vector<vector<string>> patternClauseResults;
	evaluateSelectClause(queryObj, selectClauseResults);

	if (queryObj.suchThatClauses.size() >0) {
		// if rel ref == parent
		// else if rel ref = modifies.. etc etc
		string relRef = queryObj.suchThatClauses[0].relRef;


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
	}

	if (queryObj.patternClauses.size() >0) {
		evaluatePatternClause(queryObj, patternClauseResults);
	}

	//suchThatResults vector is empty but queryObj parser is not empty OR patternResults vector is empty but queryObj parser is not empty
	if ((suchThatResults.empty() && !(queryObj.suchThatClauses.empty())) || (patternClauseResults.empty() && !(queryObj.patternClauses.empty()))) { //meaningless queries or pattern or such that supposed to return stuff but nothing 
		return; // returns nothing. Takes into account meaningless queries for 1. queries with suchThat OR pattern, 2. queries with suchThat AND pattern
	}

	else if ((suchThatResults.empty() && queryObj.suchThatClauses.empty()) && (patternClauseResults.empty() && queryObj.patternClauses.empty())) { // only select clause exists
		for (string row : selectClauseResults) { //only select clause
			output.push_back(row);
		}
		return;
	}
	else {
		if (queryObj.suchThatClauses.size() > 0 && queryObj.patternClauses.size() > 0) {
			filterSuchThatPatternClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
		else if (queryObj.patternClauses.size() > 0) {
			filterOnlyPatternClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
		else { //suchThat
			filterOnlySuchThatClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
	}
	
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




