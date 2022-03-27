#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"



using namespace std;

struct ClauseMaster {
	vector<string> Col1;
	vector<string> Col2;
	vector<string> Col3;
};

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	// method for evaluating a query
	void evaluate(Query query, vector<string>& results);
	void evaluateSuchThatClause(Query query, vector<vector<string>>& suchThatResults, vector<ClauseMaster>& clausesList);
	void evaluatePatternClause(Query query, vector<vector<string>>& patternClauseResults, vector<ClauseMaster>& clausesList);
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

	void addClausesList(vector<string> Col1, vector<string> Col2, vector<string> Col3);
	vector<ClauseMaster> clausesList;


};