#include <unordered_set>

#include "QuerySorter.h"

void QuerySorter::sort(Query& query, vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup)
{
	vector<Clause> tempMeaningfulGroup;
	vector<Clause> tempMeaninglessGroup;

	categorize(query, tempMeaningfulGroup, tempMeaningfulGroup);
	//prioritize(tempMeaningfulGroup, tempMeaningfulGroup);
}

//int QuerySorter::getPriority(shared_ptr<Clause> clause)
//{
//	if (clause->getType() == CLAUSE_PATTERN) {
//		return 5;
//	}
//
//	auto suchThat = dynamic_pointer_cast<SuchThatClause>(clause);
//
//	if (suchThat->relRef == "Calls") {
//		return 0;
//	}
//	else if (suchThat->relRef == "Parent") {
//		return 1;
//	}
//	else if (suchThat->relRef == "Next") {
//		return 2;
//	}
//	else if (suchThat->relRef == "Modifies") {
//		return 3;
//	}
//	else if (suchThat->relRef == "Uses") {
//		return 4;
//
//	} else if (suchThat->relRef == "Calls*") {
//		return 6;
//	}
//	else if (suchThat->relRef == "Parent*") {
//		return 7;
//	}
//	else if (suchThat->relRef == "Next*") {
//		return 8;
//	}
//	else if (suchThat->relRef == "Modifies*") {
//		return 9;
//	}
//	else if (suchThat->relRef == "Uses*") {
//		return 10;
//	}
//
//	return -1;
//}

void QuerySorter::categorize(Query& query, vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup)
{
	unordered_set<string> selectSynonyms;
	for (SelectClause select : query.selectClauses) {
		selectSynonyms.insert(select.name);
	}

	for (SuchThatClause clause : query.suchThatClauses) {
		if (selectSynonyms.find(clause.firstArgument[1]) != selectSynonyms.end()) {
			meaningfulGroup.push_back(clause);
		}
		else if (selectSynonyms.find(clause.secondArgument[1]) != selectSynonyms.end()) {
			meaningfulGroup.push_back(clause);
		}
		else {
			meaninglessGroup.push_back(clause);
		}
	}

	for (PatternClause clause : query.patternClauses) {
		if (selectSynonyms.find(clause.patternSynonym) != selectSynonyms.end()) {
			meaningfulGroup.push_back(clause);
		}
		else if (selectSynonyms.find(clause.LHS[1]) != selectSynonyms.end()) {
			meaningfulGroup.push_back(clause);
		}
		else {
			meaninglessGroup.push_back(clause);
		}
	}
}

//void QuerySorter::prioritize(vector<Clause>& meaningfulGroup, vector<Clause>& meaninglessGroup)
//{
//	std::sort(meaningfulGroup.begin(), meaningfulGroup.end(), [&](const Clause a, const Clause b) {
//		auto first = make_shared<Clause>(a);
//		auto second = make_shared<Clause>(b);
//		return getPriority(first) < getPriority(second);
//	});\
//
//	std::sort(meaninglessGroup.begin(), meaninglessGroup.end(), [&](const Clause a, const Clause b) {
//		auto first = make_shared<Clause>(a);
//		auto second = make_shared<Clause>(b);
//		return getPriority(first) < getPriority(second);
//	});
//}
