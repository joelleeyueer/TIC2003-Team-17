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
	static void insertAssignment(string assignmentLine);

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

	// method to get all the procedures from the database
	static void getProcedure(vector<string>& results);

	static void getConstant(vector<string>& results);

	static void getVariable(vector<string>& results);

	static void getStatement(vector<string>& results);

	static void getRead(vector<string>& results);

	static void getPrint(vector<string>& results);

	static void getAssignment(vector<string>& results);

	static void getAssignmentPattern(vector<vector<string>>& results);

	static void getParent(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue);

	static void getParentT(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue);

	static void getModifies(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

	static void getUses(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue);

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

