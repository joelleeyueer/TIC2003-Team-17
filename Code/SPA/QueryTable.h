#pragma once

#include <vector>
#include <string>

using namespace std;

class QueryTable
{
public:
	QueryTable();

	vector<vector<string>> queryTable;
	vector<string> queryTableName;

	void join();




};

