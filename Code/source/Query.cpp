#include "Query.h"

Query::Query()
{
	selectClauses = vector<SelectClause>{};
}

void Query::addSelectClause(string name, string designEntity)
{
	selectClauses.push_back(SelectClause{ name, designEntity });
}
