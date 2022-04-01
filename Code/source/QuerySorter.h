#pragma once

#include <iostream>
#include <algorithm> 
#include <vector> 

#include "Query.h"
#include "Clause.h"

using namespace std;

class QuerySorter
{
public:
	void sort(Query& query, vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup);

private:
	int getPriority(shared_ptr<Clause> clause);
	void categorize(Query& query, vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup);
	void prioritize(vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup);
};

