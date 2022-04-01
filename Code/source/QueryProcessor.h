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

	// method for evaluating a query
	void evaluate(Query query, vector<string>& results);
	void evaluateSuchThatClause(Query query, vector<vector<string>>& suchThatResults);
	void evaluatePatternClause(Query query, vector<vector<string>>& patternClauseResults);
	void filterEmptyResultsClause(Query query, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults);
	void filterSuchThatPatternClause2(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterSuchThatPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterOnlySuchThatClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void filterOnlyPatternClause(Query queryObj, vector<string>& selectClauseResults, vector<vector<string>>& suchThatResults, vector<vector<string>>& patternClauseResults, vector<string>& output);
	void evaluateSelectClause(Query query, vector<string>& results, int iterator);
	void evaluatePatternClause(Query query, vector<vector<string>>& results, int iterator);
	void evaluateParentClause(Query query, vector<vector<string>>& results, int iterator);
	void evaluateParentTClause(Query query, vector<vector<string>>& results, int iterator);
	void evaluateModifiesClause(Query query, vector<vector<string>>& results, int iterator);
	void evaluateUsesClause(Query query, vector<vector<string>>& results, int iterator);

	


};