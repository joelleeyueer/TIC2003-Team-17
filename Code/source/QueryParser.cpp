#include "QueryParser.h"
#include <iostream>

#include <regex>
#include <stack>

#define INTEGER_PATTERN "[0-9]+"
#define NAME_PATTERN "[a-zA-Z][a-zA-Z0-9]*"
#define IDENT_PATTERN "[a-zA-Z][a-zA-Z0-9]*"

void QueryParser::parse(list<string> tokens, Query& query)
{
	remainingTokens = tokens;
	currentDeclarationList = {};

	parseDeclarationList(query);
	parseSelectClause(query);


	while (remainingTokens.size() > 0) {
		if (remainingTokens.front() == "such") {
			parseSuchThatClause(query);

		} else if (remainingTokens.front() == "pattern") {
			parsePatternClause(query);
		}
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
	if (remainingTokens.front() == "<") { //isMultiSelect
		expect("<");
		while (remainingTokens.front() != ">") {
			string synonym = remainingTokens.front();
			validateSynonym(synonym, true);
			next();
			currentQuery.addSelectClause(synonym, currentDeclarationList[synonym]);

			if (remainingTokens.front() == ",") {
				expect(",");
			}
		}
		expect(">");
	}

	else { //isSingleSelect
		string synonym = remainingTokens.front();
		validateSynonym(synonym, true);
		next();
		currentQuery.addSelectClause(synonym, currentDeclarationList[synonym]);
	}
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
		next();
	}
	else if (firstArgument == "_"){
		firstArgumentPair = { "undeclared", firstArgument };
		next();
	}
	else if (firstArgument == "\"") { //usesP or modifiesP
		next();
		firstArgument = remainingTokens.front();
		next();
		expect("\"");
		firstArgumentPair = { "IDENT", firstArgument };
	}
	else {
		firstArgumentPair = { "line number", firstArgument };
		next();
	}

	
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
			parseExpression(currentQuery);
			expect("\"");
			expect("_");
			expect(")");
			RHSPair = { "partial match", expr };
			expr = "";
		}
	}
	else if (RHSinitial == "\"") { //(sth,"abc")
		expect("\"");
		parseExpression(currentQuery);
		expect("\"");
		expect(")");
		RHSPair = { "exact match", expr };
		expr = "";

	}

	currentQuery.addPatternClause(patternSynonym, LHSPair, RHSPair);
}

void QueryParser::parseExpression(Query& currentQuery)
{
	stack<string> operands;
	expression.clear();

	while ((remainingTokens.front() != "\"") && (remainingTokens.front() != "_")) {
		string exprToken = remainingTokens.front();

		if (exprToken == "(") {
			operands.push(exprToken);
			next();
		}
		else if (exprToken == ")") {
			while (operands.top() != "(") {
				expression.push_back(operands.top());
				operands.pop();
				
			}
			operands.pop(); // to pop the last "("
			next();
		}
		else if (validateIdent(exprToken) || validateNumber(exprToken)) {
			expression.push_back(exprToken);
			next();
		}
		else { //PEMDAS
			if (operands.empty() || precedence(operands.top()) < precedence(exprToken)) {
				operands.push(exprToken);
				next();
			}
			else if (precedence(operands.top()) >= precedence(exprToken)) {
				expression.push_back(operands.top());
				operands.pop();
				operands.push(exprToken);
				next();
			}
		}
	}

	while (!operands.empty()) {
		expression.push_back(operands.top());
		operands.pop();
	}

	for (string i : expression) {
		expr += i;
	}

}

int QueryParser::precedence(string symbol) {
	if (symbol == "+" || symbol == "-") {
		return 0;
	}
	else if (symbol == "%" || symbol == "*" || symbol == "/") {
		return 1;
	}
	else {
		return -1;
	}
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

	else if (symbol == "call") {
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

bool QueryParser::validateNumber(string symbol)
{
	return regex_match(symbol, regex(INTEGER_PATTERN));
}