#pragma once

#include <string>
#include <vector>
#include "Database.h"
#include "Query.h"

using namespace std;

class QueryProcessor {
public:
	// default constructor
	QueryProcessor();

	// destructor
	~QueryProcessor();

	// method for evaluating a query
	void evaluate(Query query, vector<string>& results);

	void removeDupe(vector<string>& results);
};