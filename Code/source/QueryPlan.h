#pragma once

#include <iostream>
#include <algorithm> 
#include <vector> 

#include "Query.h"

using namespace std;

class QueryPlan
{
public:
	QueryPlan();

	vector<SuchThatClause> meaningfulSuchThatClause;
	vector<PatternClause> meaningfulPatternClause;
	vector<SuchThatClause> meaninglessSuchThatClause;
	vector<PatternClause> meaninglessPatternClause;

	void plan(Query& query);

private:
	int getPriority(string clauseType);
	void categorize(Query& query);
};

