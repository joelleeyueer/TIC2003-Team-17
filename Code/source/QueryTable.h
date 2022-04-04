#pragma once

#include <vector>
#include <string>
#include "Query.h"

using namespace std;

class QueryTable
{
public:
	QueryTable();

	vector<string> queryTableName;
	vector<vector<string>> queryTable;
	
	void evaluateIncomingSuchThat(SuchThatClause clause, vector<vector<string>> incomingData);
	void evaluateIncomingSuchThatOneCol(SuchThatClause clause, vector<string> incomingData, int indexToKeep);
	void evaluateIncomingPattern(PatternClause clause, vector<vector<string>> incomingData);
	void evaluateIncomingSelect(SelectClause clause, vector<string> incomingData);

	void compareSuchThatSynonym(SuchThatClause clause, bool &oneColSimilar, bool &allColSimilar);
	void compareOneColSuchThatSynonym(SuchThatClause clause, bool& similar, int indexToKeep);
	void comparePatternSynonym(PatternClause clause, bool& oneColSimilar, bool& allColSimilar);
	void compareSelectSynonym(SelectClause clause, bool& similar);

	void join(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void insert(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void crossProduct(vector<vector<string>> incomingData, vector<string> incomingSynonyms);

	// for select clauses
	void join(vector<string> incomingData, string incomingSynonym);
	void insert(vector<string> incomingData, string incomingSynonym);
	void crossProduct(vector<string> incomingData, string incomingSynonyms);
	void dropColumns(Query queryObj);
	void dropNonSynColumns(int indexToKeep, vector<string> columnToKeep, vector<vector<string>> incomingData);
	void checkSynonym(SuchThatClause clause, vector<vector<string>> incomingData, bool &SynSyn);
	void sortColumns(Query queryObj);
	void queryToOutput(vector<string>& output);
};

