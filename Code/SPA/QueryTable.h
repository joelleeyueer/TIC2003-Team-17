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
	
	void evaluate();
	void join(vector<vector<string>> incomingData);
	void insert(vector<vector<string>> incomingData);
	void crossProduct(vector<vector<string>> incomingData);





};

