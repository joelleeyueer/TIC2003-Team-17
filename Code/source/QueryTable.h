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
	
	void evaluateIncomingSuchThat(Query queryObj, vector<vector<string>> incomingData, int iterator);
	void compareSuchThatSynonym(Query queryObj, int iterator, bool &oneColSimilar, bool &allColSimilar);
	void comparePatternSynonym(Query queryObj, int iterator, bool& oneColSimilar, bool& allColSimilar);
	void join(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void insert(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void crossProduct(vector<vector<string>> incomingData, vector<string> incomingSynonyms);
	void dropColumns(Query queryObj);
	void queryToOutput(vector<string>& output);





};

