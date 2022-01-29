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
			//1. read r; select r;
			list<string> testInput = { "read", "r", ";", "Select", "r", ";" };
			list<string> expectedRemaining = { "Select", "r", ";" };

			QueryParser qp;
			qp.remainingTokens = testInput;
			qp.parseDeclarationList();

			auto it = qp.remainingTokens.begin();
			for (int i = 0; i < qp.remainingTokens.size(); i++) {
				Logger::WriteMessage((*it).c_str());
				++it;
			}

			Assert::IsTrue(qp.currentDeclarationList.find("r") != qp.currentDeclarationList.end()); //if r is not inside the declaration list
			Assert::IsTrue(qp.currentDeclarationList["r"] == "read");

			}
		}

		

		// Some private helper functions can be added below.
	private:
	};
}