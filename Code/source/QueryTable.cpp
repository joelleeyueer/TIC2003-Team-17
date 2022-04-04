
#include "QueryTable.h"
#include <string>
#include <vector>
#include <unordered_set>

QueryTable::QueryTable()
{
	vector<vector<string>> queryTable = {};
}

void QueryTable::evaluateIncomingSuchThat(SuchThatClause clause, vector<vector<string>> incomingData)
{
	//note: all incoming queries will have at least ONE matching Select synonym
	//if it doesnt match, program auto terminate
	bool oneColSimilar = false;
	bool allColSimilar = false;
	bool synSyn = true;

	//checkSynonym(clause, incomingData, synSyn);

	//if (synSyn == false) return;

	compareSuchThatSynonym(clause, oneColSimilar, allColSimilar);

	if (queryTable.empty()) {
		queryTable = incomingData;
		queryTableName.push_back(clause.firstArgument[1]);
		queryTableName.push_back(clause.secondArgument[1]);
	}
	else {
		if (oneColSimilar == true && allColSimilar == true) {
			join(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
		}
		else if (oneColSimilar == true && allColSimilar == false) {
			insert(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
		}
		else if (oneColSimilar == false && allColSimilar == false) {
			crossProduct(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
		}
	}

	//if (queryTable.empty()) {
	//	if (bothIsSynonym(clause)) {
	//		queryTable = incomingData;
	//		queryTableName.push_back(clause.firstArgument[1]);
	//		queryTableName.push_back(clause.secondArgument[1]);
	//	}
	//	else {
	//		// if there's only one synonym
	//		// find out which column is the synonym
	//		int indexToKeep = 0;
	//		if (clause.secondArgument[0] != "undeclared" && clause.secondArgument[0] != "line number" && clause.secondArgument[0] != "IDENT") {
	//			indexToKeep = 1;
	//			queryTableName.push_back(clause.secondArgument[1]);
	//		}
	//		else {
	//			queryTableName.push_back(clause.firstArgument[1]);
	//		}
	//		
	//		// store the column with the synonym in a multidimensional array
	//		vector<vector<string>> tempTable(incomingData.size(), vector<string>(1));

	//		for (int row = 0; row < incomingData.size(); row++) {
	//			tempTable[row][0] = incomingData[row][indexToKeep];
	//		}

	//		queryTable = tempTable;
	//	}
	//}
	//else { // if query table is not empty
	//	if (bothIsSynonym(clause)) {
	//		if (oneColSimilar == true && allColSimilar == true) {
	//			join(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
	//		}
	//		else if (oneColSimilar == true && allColSimilar == false) {
	//			insert(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
	//		}
	//		else if (oneColSimilar == false && allColSimilar == false) {
	//			crossProduct(incomingData, { clause.firstArgument[1], clause.secondArgument[1] });
	//		}
	//	}
	//	else {
	//		// if there's only one synonym
	//		int indexToKeep = 0;
	//		if (clause.secondArgument[0] != "undeclared" && clause.secondArgument[0] != "line number" && clause.secondArgument[0] != "IDENT") {
	//			indexToKeep = 1;
	//			queryTableName.push_back(clause.secondArgument[1]);
	//		}
	//		else {
	//			queryTableName.push_back(clause.firstArgument[1]);
	//		}

	//		vector<string> columnToKeep;
	//		dropNonSynColumns(indexToKeep, columnToKeep, incomingData);

	//		if (oneColSimilar == true && allColSimilar == true) {
	//			join(columnToKeep, queryTableName[queryTableName.size()-1]);
	//		}
	//		else if (oneColSimilar == true && allColSimilar == false) {
	//			insert(columnToKeep, queryTableName[queryTableName.size() - 1]);
	//		}
	//		else if (oneColSimilar == false && allColSimilar == false) {
	//			crossProduct(columnToKeep, queryTableName[queryTableName.size() - 1]);
	//		}
	//	}	
	//}
}

void QueryTable::evaluateIncomingSuchThatOneCol(SuchThatClause clause, vector<string> incomingData, int indexToKeep)
{
	bool similar = false;

	compareOneColSuchThatSynonym(clause, similar, indexToKeep);

	if (queryTable.empty()) {
		vector<vector<string>> tempTable(incomingData.size(), vector<string>(1));

		for (int row = 0; row < incomingData.size(); row++) {
			tempTable[row][0] = incomingData[row];
		}
		if (indexToKeep == 1) {
			queryTableName.push_back(clause.firstArgument[1]);
		}
		else {
			queryTableName.push_back(clause.secondArgument[1]);
		}
		queryTable = tempTable;
	}
	else {
		if (indexToKeep == 0) {
			if (similar != true) {
				crossProduct(incomingData, clause.firstArgument[1]);
			}
			else {
				join(incomingData, clause.firstArgument[1]);
			}
		}
		else {
			if (similar != true) {
				crossProduct(incomingData, clause.secondArgument[1]);
			}
			else {
				join(incomingData, clause.secondArgument[1]);
			}
		}
		
	}
}

void QueryTable::evaluateIncomingPattern(PatternClause clause, vector<vector<string>> incomingData)
{
	//note: all incoming queries will have at least ONE matching Select synonym
	//if it doesnt match, program auto terminate
	bool oneColSimilar = false;
	bool allColSimilar = false;

	comparePatternSynonym(clause, oneColSimilar, allColSimilar);

	if (queryTable.empty()) {
		queryTable = incomingData;
		queryTableName.push_back(clause.patternSynonym);
		queryTableName.push_back(clause.LHS[1]);
	}

	else {
		if (oneColSimilar == true && allColSimilar == true) {
			join(incomingData, { clause.patternSynonym, clause.LHS[1] });
		}
		else if (oneColSimilar == true && allColSimilar == false) {
			insert(incomingData, { clause.patternSynonym, clause.LHS[1] });
		}
		else if (oneColSimilar == false && allColSimilar == false) {
			crossProduct(incomingData, { clause.patternSynonym, clause.LHS[1] });
		}
	}
}

void QueryTable::evaluateIncomingSelect(SelectClause clause, vector<string> incomingData)
{
	bool similar = false;

	compareSelectSynonym(clause, similar);

	if (queryTable.empty()) {
		vector<vector<string>> tempTable(incomingData.size(), vector<string>(1));

		for (int row = 0; row < incomingData.size(); row++) {
			tempTable[row][0] = incomingData[row];
		}

		queryTableName.push_back(clause.name);
		queryTable = tempTable;
	}
	else {
		if (similar != true) {
			crossProduct(incomingData, clause.name);
		}
	}
}

void QueryTable::compareSuchThatSynonym(SuchThatClause clause, bool& oneColSimilar, bool& allColSimilar)
{
	if (queryTableName.size() == 0) {
		return;
	}

	int count = 0;
	for (string synonym : queryTableName) {
		if (clause.firstArgument[1] == synonym) {
			count++;
		}
		if (clause.secondArgument[1] == synonym) {
			count++;
		}
	}

	int selectClausesQty = queryTableName.size();
	if (count >= selectClausesQty) {
		oneColSimilar = true;
		allColSimilar = true;
	}

	else if (count > 0) {
		oneColSimilar = true;
	}
}

void QueryTable::compareOneColSuchThatSynonym(SuchThatClause clause, bool& similar, int indexToKeep)
{
	if (queryTableName.size() == 0) {
		return;
	}
	
	if (indexToKeep == 0) {
		for (string synonym : queryTableName) {
			if (clause.firstArgument[1] == synonym) {
				similar = true;
				break;
			}
		}
	}

	else {
		for (string synonym : queryTableName) {
			if (clause.secondArgument[1] == synonym) {
				similar = true;
				break;
			}
		}
	}
}

void QueryTable::comparePatternSynonym(PatternClause clause, bool& oneColSimilar, bool& allColSimilar)
{
	int count = 0;
	for (string synonym : queryTableName) {
		if (clause.patternSynonym == synonym) {
			count++;
		}
		if (clause.LHS[1] == synonym) {
			count++;
		}
	}

	if (count == 0) {
		return;
	}
	else if (count > 0) {
		oneColSimilar = true;
		if (queryTableName.size() == count) {
			allColSimilar = true;
		}
	}
}

void QueryTable::compareSelectSynonym(SelectClause clause, bool& similar)
{
	if (queryTableName.size() == 0) {
		return;
	}

	for (string synonym : queryTableName) {
		if (clause.name == synonym) {
			similar = true;
			break;
		}
	}
}

void QueryTable::join(vector<vector<string>> incomingData, vector<string> incomingSynonyms)
{
	bool swap = false;
	if (incomingSynonyms[0] != queryTableName[0]) {
		swap = true;
	}

	vector<vector<string>> tempJoinTable;
	//oneColSimilar == true && allColSimilar == true
	for (vector<string> incomingRow : incomingData) {
		for (vector<string> queryTableRow : queryTable) {
			if (swap) {
				if (incomingRow[0] == queryTableRow[1] && incomingRow[1] == queryTableRow[0]) {
					tempJoinTable.push_back({ incomingRow[1], incomingRow[0] });
				}
			}
			else {
				if (incomingRow[0] == queryTableRow[0] && incomingRow[1] == queryTableRow[1]) {
					tempJoinTable.push_back({ incomingRow[0], incomingRow[1] });
				}
			}
		}
	}

	queryTable.clear();
	queryTable = tempJoinTable;
}

void QueryTable::insert(vector<vector<string>> incomingData, vector<string> incomingSynonyms)
{
	//find the synonym that is similar in incoming data and current table and map their index i.e. <a, v>  incoming and <v, d> current so we map 1 -> 0
	int similarIndexIncoming = 0;
	int similarIndexCurrent = 0;

	for (int currentIndex = 0; currentIndex < queryTableName.size(); currentIndex++) {
		if (queryTableName[currentIndex] == incomingSynonyms[0]) {
			similarIndexIncoming = 0;
			similarIndexCurrent = currentIndex;
			break;
		} 
		else if (queryTableName[currentIndex] == incomingSynonyms[1]) {
			similarIndexIncoming = 1;
			similarIndexCurrent = currentIndex;
			break;
		}
	}

	//add the uncommon synonym in the incoming data to the list of current query table names
	queryTableName.push_back(incomingSynonyms[abs(similarIndexIncoming - 1)]);

	//reduce the number of rows in the query table by finding the intersection between the similar column rows in query table and in incoming data
	vector<vector<string>> tempIntersectTable;
	for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
		for (int incomingRow = 0; incomingRow < incomingData.size(); incomingRow++) {
			if (queryTable[currentRow][similarIndexCurrent] == incomingData[incomingRow][similarIndexIncoming]) {
				//if the current query table row value == incoming row value, then we need to add that to the temp table
				//so we add the contents of the current query table row first 
				tempIntersectTable.push_back(queryTable[currentRow]);
				//then we add the extra value in the incoming data that shares no common column with the current query table
				//abs(similarIndexIncoming-1) because there's only two columns in incoming data and one is common, so the other confirm not common
				tempIntersectTable[tempIntersectTable.size()-1].push_back(incomingData[incomingRow][abs(similarIndexIncoming -1)]);
			}
		}
	}

	queryTable.clear();
	queryTable = tempIntersectTable;
}

void QueryTable::crossProduct(vector<vector<string>> incomingData, vector<string> incomingSynonyms)
{
	// add the incoming data synonynm to the current query table synonym
	queryTableName.push_back(incomingSynonyms[0]);
	queryTableName.push_back(incomingSynonyms[1]);

	vector<vector<string>> tempCrossProductTable;
	// find whether the incoming data or the current query table has more rows, the one that has more rows need to be the outer for loop. else, the cartesian product will fail because not all the rows would be added
	if (queryTable.size() >= incomingData.size()) {
		for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
			// if the current query table has more data than the incoming data
			for (int incomingRow = 0; incomingRow < incomingData.size(); incomingRow++) {
				// add the current query table columns to the temp table first to preserve the order (order in query table need to be == order in query table name)
				tempCrossProductTable.push_back(queryTable[currentRow]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow][0]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow][1]);
			}
		}
	}
	else {
		for (int incomingRow = 0; incomingRow < incomingData.size(); incomingRow++) {
			// if the incoming data has more data than the current query table
			for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
				// add the current query table columns to the temp table first to preserve the order (order in query table need to be == order in query table name)
				tempCrossProductTable.push_back(queryTable[currentRow]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow][0]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow][1]);
			}
		}
	}
	queryTable.clear();
	queryTable = tempCrossProductTable;
}

void QueryTable::join(vector<string> incomingData, string incomingSynonym)
{
	vector<vector<string>> tempJoinTable;
	for (int row = 0; row < queryTable.size(); row++) {
		if (queryTable[row][0] == incomingData[row]) {
			tempJoinTable.push_back(queryTable[row]);
		}
	}

	queryTable.clear();
	queryTable = tempJoinTable;
}

void QueryTable::insert(vector<string> incomingData, string incomingSynonym)
{
	// map the similar synonym in the incoming data to the correct column with the same synonym in the queryTable
	int similarColumn = 0;
	for (int currentIndex = 0; currentIndex < queryTableName.size(); currentIndex++) {
		if (incomingSynonym == queryTableName[currentIndex]) {
			similarColumn = currentIndex;
		}
	}

	vector<vector<string>> tempInsertTable;
	for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
		if (queryTable[currentRow][similarColumn] == incomingData[currentRow]) {
			tempInsertTable.push_back(queryTable[currentRow]);
		}
	}

	queryTable.clear();
	queryTable = tempInsertTable;
}

void QueryTable::crossProduct(vector<string> incomingData, string incomingSynonym)
{
	//add the extra synonym to the current query table
	queryTableName.push_back(incomingSynonym);

	vector<vector<string>> tempCrossProductTable;

	if (queryTable.size() >= incomingData.size()) {
		for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
			for (int incomingRow = 0; incomingRow < incomingData.size(); incomingRow++) {
				tempCrossProductTable.push_back(queryTable[currentRow]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow]);
			}
		}
	}
	else {
		for (int incomingRow = 0; incomingRow < incomingData.size(); incomingRow++) {
			for (int currentRow = 0; currentRow < queryTable.size(); currentRow++) {
				tempCrossProductTable.push_back(queryTable[currentRow]);
				tempCrossProductTable[tempCrossProductTable.size() - 1].push_back(incomingData[incomingRow]);
			}
		}
	}

	queryTable.clear();
	queryTable = tempCrossProductTable;
}

void QueryTable::dropColumns(Query queryObj)
{
	// find columns to keep
	unordered_set<int> indexOfColumnsToKeep;
	for (int i = 0; i < queryTableName.size(); i++) {
		for (int j = 0; j < queryObj.selectClauses.size(); j++) {
			if (queryTableName[i] == queryObj.selectClauses[j].name) {
				indexOfColumnsToKeep.insert(i);
				break;
			}
		}
	}

	if (indexOfColumnsToKeep.size() == queryTableName.size()) {
		return;
	}

	// actually drop the columns
	vector<vector<string>> tempTable;
	for (int row = 0; row < queryTable.size(); row++) {
		vector<string> tempRow;
		for (int column = 0; column < queryTable[row].size(); column++) {
			if (indexOfColumnsToKeep.find(column) != indexOfColumnsToKeep.end()) { // if the column index is in the index of columns to keep, add it to the tempRow
				tempRow.emplace_back(queryTable[row][column]);
			}
		}
		tempTable.emplace_back(tempRow);
	}

	queryTable.clear();
	queryTable = tempTable;
}

void QueryTable::dropNonSynColumns(int indexToKeep, vector<string> columnToKeep, vector<vector<string>> incomingData)
{
	vector<string> tempColumnToKeep;

	// actually drop the columns
	for (int row = 0; row < incomingData.size(); row++) {
		tempColumnToKeep.push_back(incomingData[row][indexToKeep]);
	}
	columnToKeep = tempColumnToKeep;
}

void QueryTable::checkSynonym(SuchThatClause clause, vector<vector<string>> incomingData, bool &synSyn)
{
	if ((clause.firstArgument[0] != "undeclared" && clause.firstArgument[0] != "line number" && clause.firstArgument[0] != "IDENT") && (clause.secondArgument[0] != "undeclared" && clause.secondArgument[0] != "line number" && clause.secondArgument[0] != "IDENT")) {
		return;
	}

	else {
		synSyn = false;
		int indexToKeep = 1;
		vector<string> tempColumnToKeep;

		if (clause.firstArgument[0] != "undeclared" && clause.firstArgument[0] != "line number" && clause.firstArgument[0] != "IDENT") {
			indexToKeep = 0;
		 }

		for (int row = 0; row < incomingData.size(); row++) {
			tempColumnToKeep.push_back(incomingData[row][indexToKeep]);
		}

		evaluateIncomingSuchThatOneCol(clause, tempColumnToKeep, indexToKeep);



	}
}

void QueryTable::sortColumns(Query queryObj)
{
	vector<string> TEMPqueryTableName;
	vector<vector<string>> TEMPqueryTable;

	for (int scIter = 0; scIter < queryObj.selectClauses.size(); scIter++) {
		for (int col = 0; col < queryTableName.size(); col++) {
				if (queryObj.selectClauses[scIter].name == queryTableName[col]) {
					TEMPqueryTableName.push_back(queryTableName[col]);
					for (int row = 0; row < queryTable.size(); row++) {
						TEMPqueryTable[scIter].push_back(queryTable[row][col]);
					}
				}
		}
	}
	queryTableName.clear();
	queryTable.clear();
	queryTableName = TEMPqueryTableName;
	queryTable = TEMPqueryTable;
}

void QueryTable::queryToOutput(vector<string>& output)
{
	//compare selectclause synonym with the query table name and sort accordingly 
	for (int row = 0; row < queryTable.size(); row++) {
		string temp = "";
		for (int column = 0; column < queryTable[row].size(); column++) {
			if (column == queryTable[row].size() - 1) {
				temp += queryTable[row][column];
			}
			else {
				temp += queryTable[row][column] + " ";
			}
		}
		output.push_back(temp);
	} 
}
