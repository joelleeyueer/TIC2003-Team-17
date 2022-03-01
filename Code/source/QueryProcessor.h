#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"



using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	// method for evaluating a query
	void evaluate(Query query, vector<string>& results);
	void evaluateSelectClause(Query query, vector<string>& results);
	void evaluatePatternClause(Query query, vector<vector<string>>& results);
	void evaluateParentClause(Query query, vector<vector<string>>& results);
	void evaluateParentTClause(Query query, vector<vector<string>>& results);
	void evaluateModifiesClause(Query query, vector<vector<string>>& results);
	void evaluateUsesClause(Query query, vector<vector<string>>& results);
};