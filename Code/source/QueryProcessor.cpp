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
QueryProcessor::QueryProcessor() {
	clausesList = vector<ClauseMaster>{};
}

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

	evaluateSelectClause(queryObj, selectClauseResults, 0);
	evaluateSuchThatClause(queryObj, suchThatResults, clausesList);
	evaluatePatternClause(queryObj, suchThatResults, clausesList);


	//suchThatResults vector is empty but queryObj parser is not empty OR patternResults vector is empty but queryObj parser is not empty
	if ((clausesList.empty() && !(queryObj.suchThatClauses.empty())) || (clausesList.empty() && !(queryObj.patternClauses.empty()))) { //meaningless queries or pattern or such that supposed to return stuff but nothing 
		return; // returns nothing. Takes into account meaningless queries for 1. queries with suchThat OR pattern, 2. queries with suchThat AND pattern
	}

	else if ((clausesList.empty() && queryObj.suchThatClauses.empty()) && (clausesList.empty() && queryObj.patternClauses.empty())) { // only select clause exists
		for (string row : selectClauseResults) { //only select clause
			output.push_back(row);
		}
		return;
	}
	else {
		//QP/database will return nothing when
		//Meaningful query but no answer
		//Meaningless query but no answer AKA false
		filterEmptyResultsClause(queryObj, suchThatResults, patternClauseResults);

		//everything else from this point on is a meaningful query or a meaningless query equating true
		if (queryObj.suchThatClauses.size() > 0 && queryObj.patternClauses.size() > 0) {
			//filterSuchThatPatternClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
			filterSuchThatPatternClause2(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
		else if (queryObj.patternClauses.size() > 0) {
			filterOnlyPatternClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
		else { //suchThat
			filterOnlySuchThatClause(queryObj, selectClauseResults, suchThatResults, patternClauseResults, output);
		}
	}
	
}

void QueryProcessor::evaluateSuchThatClause(Query queryObj, vector<vector<string>>& suchThatResults, vector<ClauseMaster>& clausesList)
{
	/// <summary>
	/// Iterate through suchThatResults and parse results into suchThatMaster
	/// </summary>

	int suchThatCount = queryObj.suchThatClauses.size();
	int iterator = -1;

	while (queryObj.suchThatClauses.size() > 0 && suchThatCount > 0) {
		iterator++;
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


		if (suchThatResults.size() > 0) { //meaningless or not, there are results pulled from the Database class
			for (vector<string> i : suchThatResults) {
				//check if its meaningless. if yes, dont addClausesList
				string firstArgumentType = queryObj.suchThatClauses[iterator].firstArgument[0]; //design entity, line number, _, IDENT)
				string firstArgumentSynonym = queryObj.suchThatClauses[iterator].firstArgument[1];
				if (firstArgumentType == "undeclared" || firstArgumentType == "line number" || firstArgumentType == "IDENT") {
					vector<string> col1 = { i[0] };
					vector<string> col2 = { i[1] };
					vector<string> col3 = {};

					addClausesList(col1, col2, col3);
				}
				else { //it's a design entity. check if Select synonym == Such That synonym
					bool Meaningful = 0;
					for (SelectClause i : queryObj.selectClauses) {
						if (firstArgumentSynonym == i.name) {
							Meaningful = 1;
						}
					}

					if (Meaningful == 1) {
						vector<string> col1 = { i[0] };
						vector<string> col2 = { i[1] };
						vector<string> col3 = {};

						addClausesList(col1, col2, col3);
					}
					else {
						continue;
					}
				}
				
			}
		}
		else {
			//meaningful or meaningless queries that return nothing, 
			return;
		}


		suchThatResults.clear();
		suchThatCount--;
	}
}

void QueryProcessor::evaluatePatternClause(Query queryObj, vector<vector<string>>& patternClauseResults, vector<ClauseMaster>& clausesList)
{
	/// <summary>
	/// Iterate through patternClauseResults and parse results into patternClauseMaster
	/// </summary>

	int patternCount = queryObj.patternClauses.size();
	int iterator = -1;

	while (queryObj.patternClauses.size() > 0 && patternCount > 0) {
		iterator++;
		evaluatePatternClause(queryObj, patternClauseResults, iterator);

		for (vector<string> i : patternClauseResults) {

			vector<string> col1 = { i[0] };
			vector<string> col2 = { i[1] };
			vector<string> col3 = { i[2] };
			addClausesList(col1, col2, col3);
		}

		patternClauseResults.clear();
		patternCount--;
	}
}

void QueryProcessor::filterEmptyResultsClause(Query query, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults)
{
	//if there is an empty vector from suchThatResults or patternClauseResults, the query should return nothing;
	for (auto i : suchThatResults) {
		if (i.empty()) {
			return;
		}
	}

	for (auto i : patternClauseResults) {
		if (i.empty()) {
			return;
		}
	}
}

void QueryProcessor::filterSuchThatPatternClause2(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output)
{
	//WIP resume after implementing vector 3x
	vector<vector<string>> parseResults;
	int j = -1;
	//loop through suchThatResults
	for (vector<string> i : suchThatResults) {
		j++;
		if (queryObj.selectClauses[0].name != queryObj.suchThatClauses[j].firstArgument[1]) { //check if meaningless i.e. Select a Modifies (b,..)
			continue;
		}
		else { //not meaningless
			if (queryObj.selectClauses[0].designEntity == "assign") {
				//parseResults[j].push_back(i[0]);
			}
			else { // designEntity == "variable"
				//parseResults[j].push_back(i[1]);
			}
			
		}
	}

	j = -1;
	//loop through suchThatResults
	for (auto i : patternClauseResults) {
		j++;
		if (queryObj.selectClauses[0].name != queryObj.patternClauses[j].patternSynonym) { //check if meaningless i.e. Select a pattern b (a,..)
			continue;
		}
		else {
			//parseResults[j].push_back(i[0]);

		}
	}


	return;
	//find intersection between all results in parseResults
	//WIP


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

void QueryProcessor::evaluateSelectClause(Query query, vector<string>& results, int iterator)
{
	SelectClauseEvaluator selectClauseEvaluator;
	selectClauseEvaluator.evaluate(results, query.selectClauses[0].designEntity); 
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

void QueryProcessor::addClausesList(vector<string> Col1, vector<string> Col2, vector<string> Col3)
{
	clausesList.push_back(ClauseMaster{ Col1, Col2, Col3});
}




