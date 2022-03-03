#include "QueryParser.h"
#include <iostream>

#include <regex>

#define INTEGER_PATTERN "[0-9]+"
#define NAME_PATTERN "[a-zA-Z][a-zA-Z0-9]*"
#define IDENT_PATTERN "[a-zA-Z][a-zA-Z0-9]*"

void QueryParser::parse(list<string> tokens, Query& query)
{
	remainingTokens = tokens;
	currentDeclarationList = {};

	parseDeclarationList(query);
	parseSelectClause(query);

	if (remainingTokens.size() == 0) {
		return;
	}

	if (remainingTokens.front() == "such") {
		parseSuchThatClause(query);
	}

	if (remainingTokens.size() == 0) {
		return;
	}

	if (remainingTokens.front() == "pattern") {
		parsePatternClause(query);
	}
}

void QueryParser::expect(string symbol)
{
	if (match(symbol)) {
		next();
		return;
	}

	std::throw_with_nested("expected: '" + symbol + "', got '" + remainingTokens.front() + "' instead");
}

bool QueryParser::match(string symbol)
{
	if (symbol == remainingTokens.front()) {
		return 1;
	}

	return 0;
}

void QueryParser::next()
{
	if (!remainingTokens.empty()) {
		remainingTokens.pop_front();
	}
}

void QueryParser::parseDeclarationList(Query& query)
{
	while (remainingTokens.front() != "Select") {
		string designEntity = remainingTokens.front();
		validateDesignEntity(designEntity);
		next();
		string synonym = remainingTokens.front();
		validateSynonym(synonym, false);
		currentDeclarationList[synonym] = designEntity;
		next();
		string separator = remainingTokens.front();
		while (separator == ",") {
			expect(",");
			string nextSynonym = remainingTokens.front();
			validateSynonym(synonym, false);
			currentDeclarationList[nextSynonym] = designEntity;
			next();
			separator = remainingTokens.front();
		}
		expect(";");
			
	}
	query.addDeclarationList(currentDeclarationList);
}

void QueryParser::parseSelectClause(Query& currentQuery)
{
	expect("Select");
	string synonym = remainingTokens.front();
	validateSynonym(synonym, true);
	next();
	currentQuery.addSelectClause(synonym, currentDeclarationList[synonym]);
}

void QueryParser::parseSuchThatClause(Query& currentQuery)
{
	expect("such");
	expect("that");
	string relRef = remainingTokens.front();
	next();

	if (remainingTokens.front() == "*") {
		next();
		relRef = relRef + "*";
	}

	expect("(");
	vector<string> firstArgumentPair;
	vector<string> secondArgumentPair;
	string firstArgument = remainingTokens.front();

	if (validateSynonym(firstArgument, true)) {
		firstArgumentPair = { currentDeclarationList[firstArgument], firstArgument };
	}
	else if (firstArgument == "_"){
		firstArgumentPair = { "undeclared", firstArgument };
	}
	else {
		firstArgumentPair = { "line number", firstArgument };
	}

	next();
	expect(",");
	string secondArgument = remainingTokens.front();

	if (validateSynonym(secondArgument, true)) {
		secondArgumentPair = { currentDeclarationList[secondArgument], secondArgument };
		next();
	}
	else if (secondArgument == "_"){
		secondArgumentPair = { "undeclared", secondArgument };
		next();
	}
	else if (regex_match(secondArgument, regex(INTEGER_PATTERN))) {
		secondArgumentPair = { "line number", secondArgument };
		next();
	}
	else {
		expect("\"");
		secondArgument = remainingTokens.front();
		secondArgumentPair = { "IDENT", secondArgument };
		next();
		expect("\"");
	}

	currentQuery.addSuchThatClause(relRef, firstArgumentPair, secondArgumentPair);
	expect(")");
}

void QueryParser::parsePatternClause(Query& currentQuery)
{
	vector<string> LHSPair;
	vector<string> RHSPair;
	expect("pattern");
	string patternSynonym = remainingTokens.front();
	next();
	expect("(");
	string LHSinitial = remainingTokens.front();
	if (LHSinitial == "\"") {
		next();
		string IDENT = remainingTokens.front();
		next();
		expect("\"");
		LHSPair = { "IDENT", IDENT };
	}
	else if (LHSinitial == "_") {
		LHSPair = { "undeclared", LHSinitial };
		next();
	}
	else { //synonym
		LHSPair = { currentDeclarationList[LHSinitial], LHSinitial };
		next();
	}

	expect(",");
	string RHSinitial = remainingTokens.front();
	if (RHSinitial == "_") {
		next();
		string RHSnext = remainingTokens.front();
		if (RHSnext == ")") {
			RHSPair = { "undeclared", RHSnext };
			next();
		}
		else { //has to be partial match
			expect("\"");
			string factor = remainingTokens.front();
			next();
			expect("\"");
			expect("_");
			expect(")");
			RHSPair = { "partial match", factor };
		}
	}

	currentQuery.addPatternClause(patternSynonym, LHSPair, RHSPair);
}

bool QueryParser::validateSynonym(string symbol, bool checkInDeclaration)
{
	if (checkInDeclaration && currentDeclarationList.find(symbol) == currentDeclarationList.end()) {
		return false;
	}

	return true;
}

void QueryParser::validateDesignEntity(string symbol)
{
	if (symbol == "procedure") {
		return;
	}
	else if (symbol == "constant") {
		return;
	}
	else if (symbol == "variable") {
		return;
	}
	else if (symbol == "stmt") {
		return;
	}
	else if (symbol == "read") {
		return;
	}
	else if (symbol == "print") {
		return;
	}
	else if (symbol == "assign") {
		return;
	}

	else if (symbol == "while") {
		return;
	}

	else if (symbol == "if") {
		return;
	}

	else {
		//std::throw_with_nested("design entity not valid: '" + symbol + "'");
	}
}

bool QueryParser::validateIdent(string symbol)
{
	return regex_match(symbol, regex(IDENT_PATTERN));
}
