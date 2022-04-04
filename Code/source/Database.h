#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"

using namespace std;

// The Database has to be a static class due to various constraints.
// It is advisable to just add the insert / get functions based on the given examples.
class Database {
public:

	/*	Procedures
		Constant
		Variable
		Statement
		Read
		Print
		Assignment*/
		// method to connect to the database and initialize tables in the database
	static void initialize();

	// method to close the database connection
	static void close();

	// method to insert a procedure into the database
	static void insertProcedure(string procedureName);

	// method to insert a variable into the database
	static void insertVariable(string variableName);

	// method to insert a print into the database
	static void insertRead(string ReadLine);

	// method to insert a print into the database
	static void insertPrint(string printLine);

	// method to insert an assignment into the database
	static void insertAssignment(string assignmentLine, string lhs, string rhs);

	// method to insert a statement into the database
	static void insertStatement(string statementLine);

	// method to insert a constant into the database
	static void insertConstant(string constantName);

	// method to insert a while into the database
	static void insertWhile(string whileName);

	// method to insert a if into the database
	static void insertIf(string ifName);

	// method to insert a parent into the database
	//static void insertParent(string parentName);

	// method to insert a child into the database
	static void insertChild(string parentName, string childName);

	// method to insert a child into the database
	static void insertGrandchild(string grandparentName, string childName);

	// method to insert modifies into the database
	static void insertModifies(string modifiesLine, string variableN);

	// method to insert modifiesproc into the database
	static void insertModifiesproc(string modifiesProc, string variableN);

	// method to insert uses into the database
	static void insertUses(string usesLine, string variableN);

	// method to insert usesproc into the database
	static void insertUsesproc(string usesProc, string variableN);

	// method to insert calls into the database
	static void insertCalls(string proc1, string proc2);

	// method to insert callst into the database
	static void insertCallst(string proc1, string proc2);

	// method to insert nexts into the database
	static void insertNexts(string stmt1, string stmt2);

	// method to insert nextst into the database
	static void insertNextst(string stmt1, string stmt2);

	// method to insert procstmt into the database
	static void insertProcstmt(string proc, string stmt);

	// method to insert a call into the database
	static void insertCall(string callLine);

	// method to get all the procedures from the database
	static void getProcedure(vector<string>& results);

	static void getConstant(vector<string>& results);

	static void getVariable(vector<string>& results);

	static void getStatement(vector<string>& results);

	static void getRead(vector<string>& results);

	static void getPrint(vector<string>& results);

	static void getWhile(vector<string>& results);

	static void getIf(vector<string>& results);

	static void getAssignment(vector<string>& results);

	static void getCall(vector<string>& results);

	static void getAssignmentPattern(vector<vector<string>>& results);

	static void getParent(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue);

	static void getParentT(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue);

	static void getModifies(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getModifiesP(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getUses(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getUsesP(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getNext(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getNextT(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getCalls(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getCallsT(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getCallsTmodifies(vector<string>& results, string callee);

	static void getCallsTuses(vector<string>& results, string callee);

	static void getProcstmt(vector<string>& results, string stmt);

	/*	Procedures
		Constant
		Variable
		Statement
		Read
		Print
		Assignment*/

private:
	// the connection pointer to the database
	static sqlite3* dbConnection;
	// a vector containing the results from the database
	static vector<vector<string>> dbResults;
	// the error message from the database
	static char* errorMessage;
	// callback method to put one row of results from the database into the dbResults vector
	// This method is called each time a row of results is returned from the database
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static string convertToDbName(string designEntity);
};

