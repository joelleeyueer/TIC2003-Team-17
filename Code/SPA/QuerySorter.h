#pragma once

#include <iostream>
#include <algorithm> 
#include <vector>

#include "Query.h"

using namespace std;

class QuerySorter
{
public:
	void sort(vector<ClauseGroup>& groups);
private:
	int getPriority(string clauseType);

};

class ClauseGroup {
public:
	vector<string> synonyms;
	vector<Clause> clauses;
};

