#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"
#include "QueryTable.h"

using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	QueryTable queryTable;

	// method for evaluating a query
	void evaluate(Query query, vector<string>& results);

	void evaluateSuchThatClause(SuchThatClause clause, vector<vector<string>>& suchThatResults);
	void evaluatePatternClause(PatternClause clause, map<string, string> declarationList, vector<vector<string>>& patternClauseResults);
	
	void filterEmptyResultsClause(Query query, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults);
	void filterSuchThatPatternClause2(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterSuchThatPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterOnlySuchThatClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterOnlyPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	
	void evaluateSelectClause(Query query, vector<string>& results);

	void evaluatePatternClause(SuchThatClause clause, vector<vector<string>>& results);
	void evaluateParentClause(SuchThatClause clause, vector<vector<string>>& results);
	void evaluateParentTClause(SuchThatClause clause, vector<vector<string>>& results);
	void evaluateModifiesClause(SuchThatClause clause, vector<vector<string>>& results);
	void evaluateUsesClause(SuchThatClause clause, vector<vector<string>>& results);

	


};