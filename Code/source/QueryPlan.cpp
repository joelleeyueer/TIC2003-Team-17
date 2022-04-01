#include <unordered_set>

#include "QueryPlan.h"

QueryPlan::QueryPlan()
{
	meaningfulSuchThatClause = vector<SuchThatClause>{};
	meaningfulPatternClause = vector<PatternClause>{};
	meaninglessSuchThatClause = vector<SuchThatClause>{};
	meaninglessPatternClause = vector<PatternClause>{};
}

void QueryPlan::plan(Query& query)
{
	categorize(query);

	// prioritize the meaningful such that clause because meaningless doesn't matter since we won't add it to the table
	std::sort(std::begin(meaningfulSuchThatClause), std::end(meaningfulSuchThatClause), [&](const SuchThatClause& a, const SuchThatClause& b) {
		return getPriority(a.relRef) < getPriority(b.relRef);
		});
}

void QueryPlan::categorize(Query& query)
{
	unordered_set<string> selectSynonyms;
	for (SelectClause select : query.selectClauses) {
		selectSynonyms.insert(select.name);
	}

	for (SuchThatClause clause : query.suchThatClauses) {
		if (selectSynonyms.find(clause.firstArgument[1]) != selectSynonyms.end()) {
			meaningfulSuchThatClause.push_back(clause);
		}
		else if (selectSynonyms.find(clause.secondArgument[1]) != selectSynonyms.end()) {
			meaningfulSuchThatClause.push_back(clause);
		}
		else {
			meaninglessSuchThatClause.push_back(clause);
		}
	}

	for (PatternClause clause : query.patternClauses) {
		if (selectSynonyms.find(clause.patternSynonym) != selectSynonyms.end()) {
			meaningfulPatternClause.push_back(clause);
		}
		else if (selectSynonyms.find(clause.LHS[1]) != selectSynonyms.end()) {
			meaningfulPatternClause.push_back(clause);
		}
		else {
			meaninglessPatternClause.push_back(clause);
		}
	}
}

int QueryPlan::getPriority(string clauseType)
{
	if (clauseType == "Calls") {
		return 0;
	}
	else if (clauseType == "Parent") {
		return 1;
	}
	else if (clauseType == "Next") {
		return 2;
	}
	else if (clauseType == "Modifies") {
		return 3;
	}
	else if (clauseType == "Uses") {
		return 4;

	} else if (clauseType == "Calls*") {
		return 6;
	}
	else if (clauseType == "Parent*") {
		return 7;
	}
	else if (clauseType == "Next*") {
		return 8;
	}
	else if (clauseType == "Modifies*") {
		return 9;
	}
	else if (clauseType == "Uses*") {
		return 10;
	}

	return -1;
}
