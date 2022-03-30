
#include "QueryTable.h"
#include <string>
#include <vector>

QueryTable::QueryTable() {
	vector<string> queryTableName = {};
	vector<vector<string>> queryTable = {};
}

void QueryTable::evaluateIncomingSuchThat(Query queryObj, vector<vector<string>> incomingData, int iterator)
{
	//note: all incoming queries will have at least ONE matching Select synonym
	//if it doesnt match, program auto terminate
	bool oneColSimilar = false;
	bool allColSimilar = false;

	compareSuchThatSynonym(queryObj, iterator, oneColSimilar, allColSimilar);

	if (queryTable.empty()) {
		queryTable = incomingData;
		queryTableName.push_back(queryObj.suchThatClauses[iterator].firstArgument[1]);
		queryTableName.push_back(queryObj.suchThatClauses[iterator].secondArgument[1]);
	}

	else {
		if (oneColSimilar == true && allColSimilar == true) {
			join(incomingData);
		}
		else if (oneColSimilar == true && allColSimilar == false) {
			insert(incomingData);
		}
		else if (oneColSimilar == false && allColSimilar == false) {
			crossProduct(incomingData);
		}
	}
}

void QueryTable::compareSuchThatSynonym(Query queryObj, int iterator, bool &oneColSimilar, bool &allColSimilar)
{
	int count = 0;
	for (SelectClause i: queryObj.selectClauses) {
		if (queryObj.suchThatClauses[iterator].firstArgument[1] == i.name) {
			count++;
		}
		if (queryObj.suchThatClauses[iterator].secondArgument[1] == i.name) {
			count++;
		}
	}

	if (count == 0) {
		return;
	}
	else if (count > 0) {
		oneColSimilar = true;
		if (count == 2) {
			allColSimilar = true;
		}
	}
}

void QueryTable::comparePatternSynonym(Query queryObj, int iterator, bool& oneColSimilar, bool& allColSimilar)
{
	int count = 0;
	for (SelectClause i : queryObj.selectClauses) {
		if (queryObj.patternClauses[iterator].patternSynonym == i.name) {
			count++;
		}
		if (queryObj.patternClauses[iterator].LHS[1] == i.name) {
			count++;
		}
	}

	if (count == 0) {
		return;
	}
	else if (count > 0) {
		oneColSimilar = true;
		if (queryObj.selectClauses.size() == count) {
			allColSimilar = true;
		}
	}
}

void QueryTable::join(vector<vector<string>> incomingData)
{
	//oneColSimilar == true && allColSimilar == true
	for (vector<string> incomingRow: incomingData){
		for (vector<string> queryTableRow : queryTable) {

		}
	}
}

void QueryTable::insert(vector<vector<string>> incomingData)
{
}

void QueryTable::crossProduct(vector<vector<string>> incomingData)
{
}
