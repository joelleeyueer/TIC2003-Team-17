#include "stdafx.h"
#include "../source/QueryParser.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QueryParserTests
{
	// Each cpp is a set of test cases for a specific component.
	TEST_CLASS(TestQueryParser)
	{
	public:
		// Each test method is a separate test case. The name should be unique and meaningful.

		TEST_METHOD(CheckParseDeclarationList)
		{
			list<string> testInput = { "read", "a", ",", "b", ";", "print", "c", ",", "d", ";", "Select", "b", ",", "c"};
			//list<string> testInput = { "read", "a", ";", "Select", "a"};
			//list<string> testInput = { "read", "a", ",", "b", ";", "Select", "a"};
			list<string> expectedRemaining = { };

			QueryParser qp;
			qp.remainingTokens = testInput;
			qp.parseDeclarationList();

			auto it = qp.remainingTokens.begin();
			for (int i = 0; i < qp.remainingTokens.size(); i++) {
				Logger::WriteMessage((*it).c_str());
				++it;
			}

			Assert::IsTrue(qp.currentDeclarationList.find("b") != qp.currentDeclarationList.end()); //if r is not inside the declaration list
			Assert::IsTrue(qp.currentDeclarationList["b"] == "read");
			Assert::IsTrue(qp.currentDeclarationList["c"] == "print");

		}
		// Some private helper functions can be added below.
	private:
	};
}