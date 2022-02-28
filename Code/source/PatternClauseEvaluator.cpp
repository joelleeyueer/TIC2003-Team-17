#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "Database.h"
#include "PatternClauseEvaluator.h"
#include "Query.h"


using namespace std;

void PatternClauseEvaluator::evaluate(vector<vector<string>>& results, vector<string> LHSPair, vector<string> RHSPair, map<string, string> declarationList)
{
	vector<vector<string>> tempDatabaseResult;

	Database::getAssignmentPattern(tempDatabaseResult);

	if (LHSPair[0] == "undeclared") {
		evaluateLHSUnrestricted(results, tempDatabaseResult, RHSPair, declarationList);
	}

	else if (LHSPair[0] == "IDENT") {
		string LHSidentText = LHSPair[1];
		evaluateLHSIdent(results, tempDatabaseResult, LHSidentText, RHSPair, declarationList);
	}

	else {//synonym
		evaluateLHSSynonym(results, tempDatabaseResult, RHSPair, declarationList);
	}
}

void PatternClauseEvaluator::evaluateLHSIdent(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, string LHSidentText, vector<string> RHSPair, map<string, string> declarationList)
{
	if (RHSPair[0] == "undeclared") { //a(ident,_)
		for (vector<string> resultsRow : tempDatabaseResults) {
			if (resultsRow[0]==LHSidentText) {
				results.push_back(resultsRow);
			}
		}
	}

	else if (RHSPair[0] == "partial match") { //a(ident,_"_)
		for (vector<string> resultsRow : tempDatabaseResults) {
			string RHSText = RHSPair[1];
			if (resultsRow[0] == LHSidentText) {
				if (resultsRow[1].find(RHSText) != string::npos) {
					results.push_back(resultsRow);
				}
			}
		}
	}

}

void PatternClauseEvaluator::evaluateLHSSynonym(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, vector<string> RHSPair, map<string, string> declarationList)
{
	//since LHS is synonym, check RHS - SYNONYM DOES NOT MATTER RIGHT NOW because 
	if (RHSPair[0] == "undeclared") { //a(_,_)
		for (vector<string> resultsRow : tempDatabaseResults) {
			results.push_back(resultsRow);
		}
	}

	else if (RHSPair[0] == "partial match") { //a(_,_"_)
		string RHSText = RHSPair[1];
		for (vector<string> resultsRow : tempDatabaseResults) {
			if (resultsRow[1].find(RHSText) != string::npos) {
				results.push_back(resultsRow);
			}
		}
	}
}

void PatternClauseEvaluator::evaluateLHSUnrestricted(vector<vector<string>>& results, vector<vector<string>>& tempDatabaseResults, vector<string> RHSPair, map<string, string> declarationList)
{
	//since LHS is unrestricted, check RHS
	if (RHSPair[0] == "undeclared") { //a(_,_)
		for (vector<string> resultsRow : tempDatabaseResults) {
			results.push_back(resultsRow);
		}
	}

	else if (RHSPair[0] == "partial match") { //a(_,_"_)
		string RHSText = RHSPair[1];
		for (vector<string> resultsRow : tempDatabaseResults) {
			if (resultsRow[1].find(RHSText) != string::npos) {
				results.push_back(resultsRow);
			}
		}
	}
}
