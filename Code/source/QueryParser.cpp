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

	try {
		parseDeclarationList();
		parseSelectClause(query);
	}
	catch (const std::exception& ex) {
		std::throw_with_nested("error in parsing query");
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

void QueryParser::parseDeclarationList()
{
	try {
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
	}
	catch (const std::exception& ex) {
		std::throw_with_nested("error in declaration list");
	}
}

void QueryParser::parseSelectClause(Query& currentQuery)
{
	try {
		expect("Select");

		//first synonym
		string synonym = remainingTokens.front();
		validateSynonym(synonym, true);
		next();
		currentQuery.addSelectClause(synonym, currentDeclarationList[synonym]);

		//subsequent synonym
		while (!remainingTokens.empty()) {
			expect(",");
			string synonym = remainingTokens.front();
			validateSynonym(synonym, true);
			next();
			currentQuery.addSelectClause(synonym, currentDeclarationList[synonym]);
		}
		
	}
	catch (const std::exception& ex) {
		std::throw_with_nested("error in select clause");
	}
}

void QueryParser::validateSynonym(string symbol, bool checkInDeclaration)
{
	/*if (!validateIdent(symbol)) {
		std::throw_with_nested("synonym syntactically invalid: '" + symbol + "'");
	}*/

	if (checkInDeclaration && currentDeclarationList.find(symbol) == currentDeclarationList.end()) {
		std::throw_with_nested("synonym not in declaration list: '" + symbol + "'");
	}
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
	else {
		std::throw_with_nested("design entity not valid: '" + symbol + "'");
	}
}

bool QueryParser::validateIdent(string symbol)
{
	return regex_match(symbol, regex(IDENT_PATTERN));
}
