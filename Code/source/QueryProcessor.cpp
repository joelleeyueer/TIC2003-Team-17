#include "QueryProcessor.h"
#include "SelectClauseEvaluator.h"
#include "PatternClauseEvaluator.h"
#include "ParentClauseEvaluator.h"
#include "ParentTClauseEvaluator.h"
#include "ModifiesClauseEvaluator.h"
#include "UsesClauseEvaluator.h"
#include "ModifiesPClauseEvaluator.h"
#include "NextClauseEvaluator.h"
#include "NextTClauseEvaluator.h"
#include "CallsClauseEvaluator.h"
#include "UsesPClauseEvaluator.h"
#include "CallsTClauseEvaluator.h"
#include "Tokenizer.h"
#include "Database.h"
#include "QueryTable.cpp"
#include "QueryPlan.cpp"
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
	output.clear();

	QueryPlan queryPlan = QueryPlan();
	queryPlan.plan(queryObj);

	vector<vector<string>> selectClauseResults;
	vector<vector<string>> suchThatResults;
	vector<vector<string>> patternClauseResults;

	// evaluate select clause first
	for (SelectClause clause : queryObj.selectClauses) {
		evaluateSelectClause(clause, selectClauseResults);
	}

	// create query table
	vector<string> tempSelectSynonyms;
	for (SelectClause select : queryObj.selectClauses) {
		tempSelectSynonyms.push_back(select.name);
	}
	queryTable = QueryTable(tempSelectSynonyms);
	
	for (SuchThatClause clause : queryObj.suchThatClauses) {
		evaluateSuchThatClause(clause, suchThatResults);
		if (suchThatResults.empty()) {
			return;
		}

		queryTable.evaluateIncomingSuchThat(clause, suchThatResults);
	}

	for (PatternClause clause : queryObj.patternClauses) {
		evaluatePatternClause(clause, queryObj.declarationList, patternClauseResults);
		if (patternClauseResults.empty()) {
			return;
		}

		queryTable.evaluateIncomingPattern(clause, patternClauseResults);
	}

	queryTable.dropColumns(queryObj);
	queryTable.queryToOutput(output);
}

void QueryProcessor::evaluateSelectClause(SelectClause clause, vector<vector<string>>& results)
{
	SelectClauseEvaluator selectClauseEvaluator;
	selectClauseEvaluator.evaluate(results, clause.designEntity);
}

void QueryProcessor::evaluateSuchThatClause(SuchThatClause clause, vector<vector<string>>& suchThatResults)
{
	if (clause.relRef == "Parent") {
		evaluateParentClause(clause, suchThatResults);
	}
	else if (clause.relRef == "Parent*") {
		evaluateParentTClause(clause, suchThatResults);
	}
	else if (clause.relRef == "Modifies") {
		if ((clause.firstArgument[0] == "IDENT") || (clause.firstArgument[0] == "call") || (clause.firstArgument[0] == "procedure")) {
			evaluateModifiesPClause(clause, suchThatResults);
		}
		else {
			evaluateModifiesClause(clause, suchThatResults);
		}
	}
	else if (clause.relRef == "Uses") {
		if ((clause.firstArgument[0] == "IDENT") || (clause.firstArgument[0] == "call") || (clause.firstArgument[0] == "procedure")) {
			evaluateUsesPClause(clause, suchThatResults);
		}
		else {
			evaluateUsesClause(clause, suchThatResults);
		}
	}
	else if (clause.relRef == "Next") {
		evaluateNextClause(clause, suchThatResults);
	}
	else if (clause.relRef == "Next*") {
		evaluateNextTClause(clause, suchThatResults);
	}
	else if (clause.relRef == "Calls") {
		evaluateCallsClause(clause, suchThatResults);
	}
	else if (clause.relRef == "Calls*") {
		evaluateCallsTClause(clause, suchThatResults);
	}
	else {
		return;
	}

	suchThatResults.clear();
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

void QueryProcessor::evaluatePatternClause(PatternClause clause, map<string, string> declarationList, vector<vector<string>>& results)
{
	PatternClauseEvaluator patternClauseEvaluator;
	patternClauseEvaluator.evaluate(results, clause.LHS, clause.RHS, declarationList);
}

void QueryProcessor::evaluateParentClause(SuchThatClause clause, vector<vector<string>>& results)
{
	ParentClauseEvaluator parentClauseEvaluator;
	parentClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateParentTClause(SuchThatClause clause, vector<vector<string>>& results)
{
	ParentTClauseEvaluator parentTClauseEvaluator;
	parentTClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);

}

void QueryProcessor::evaluateModifiesClause(SuchThatClause clause, vector<vector<string>>& results)
{
	ModifiesClauseEvaluator modifiesClauseEvaluator;
	modifiesClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateModifiesPClause(SuchThatClause clause, vector<vector<string>>& results)
{
	ModifiesPClauseEvaluator modifiesPClauseEvaluator;
	modifiesPClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateUsesClause(SuchThatClause clause, vector<vector<string>>& results)
{
	UsesClauseEvaluator usesClauseEvaluator;
	usesClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}



void QueryProcessor::evaluateUsesPClause(SuchThatClause clause, vector<vector<string>>& results)
{
	UsesPClauseEvaluator usesPClauseEvaluator;
	usesPClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateNextClause(SuchThatClause clause, vector<vector<string>>& results)
{
	NextClauseEvaluator nextClauseEvaluator;
	nextClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateNextTClause(SuchThatClause clause, vector<vector<string>>& results)
{
	NextTClauseEvaluator nextTClauseEvaluator;
	nextTClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateCallsClause(SuchThatClause clause, vector<vector<string>>& results)
{
	CallsClauseEvaluator callsClauseEvaluator;
	callsClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}

void QueryProcessor::evaluateCallsTClause(SuchThatClause clause, vector<vector<string>>& results)
{
	CallsTClauseEvaluator callsTClauseEvaluator;
	callsTClauseEvaluator.evaluate(results, clause.firstArgument[0], clause.firstArgument[1], clause.secondArgument[0], clause.secondArgument[1]);
}






