#pragma once

#include <vector>
#include <string>
#include "Query.h"

using namespace std;

class QueryTable
{
public:
	QueryTable() = default;

	QueryTable(vector<string> selectSynonyms);

	vector<string> queryTableName;
	vector<vector<string>> queryTable;
	
	void evaluateIncomingSuchThat(SuchThatClause clause, vector<vector<string>> incomingData);
	void evaluateIncomingPattern(PatternClause clause, vector<vector<string>> incomingData);

	void compareSuchThatSynonym(SuchThatClause clause, bool &oneColSimilar, bool &allColSimilar);
	void comparePatternSynonym(PatternClause clause, bool& oneColSimilar, bool& allColSimilar);

	void join(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void insert(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void crossProduct(vector<vector<string>> incomingData, vector<string> incomingSynonyms);

	void dropColumns(Query queryObj);
	void queryToOutput(vector<string>& output);
};

