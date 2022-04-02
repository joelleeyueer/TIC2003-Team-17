#include <string>
#include "Database.h"

sqlite3* Database::dbConnection;
vector<vector<string>> Database::dbResults;
char* Database::errorMessage;

// method to connect to the database and initialize tables in the database
void Database::initialize() {
	// open a database connection and store the pointer into dbConnection
	sqlite3_open("database.db", &dbConnection);

	// drop the existing procedure table (if any)
	string dropProcedureTableSQL = "DROP TABLE IF EXISTS procedures";
	sqlite3_exec(dbConnection, dropProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a procedure table
	string createProcedureTableSQL = "CREATE TABLE procedures ( procedureName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createProcedureTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing variable table (if any)
	string dropVariableTableSQL = "DROP TABLE IF EXISTS variables";
	sqlite3_exec(dbConnection, dropVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a variable table
	string createVariableTableSQL = "CREATE TABLE variables ( variableName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createVariableTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing assignment table (if any)
	string dropAssignmentTableSQL = "DROP TABLE IF EXISTS assignments";
	sqlite3_exec(dbConnection, dropAssignmentTableSQL.c_str(), NULL, 0, &errorMessage);

	// create an assignment table
	string createAssignmentTableSQL = "CREATE TABLE assignments ( line VARCHAR(255) , lhs VARCHAR(255) , rhs VARCHAR(255));";
	sqlite3_exec(dbConnection, createAssignmentTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing print table (if any)
	string dropPrintTableSQL = "DROP TABLE IF EXISTS prints";
	sqlite3_exec(dbConnection, dropPrintTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a print table
	string createPrintTableSQL = "CREATE TABLE prints ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createPrintTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing read table (if any)
	string dropReadTableSQL = "DROP TABLE IF EXISTS reads";
	sqlite3_exec(dbConnection, dropReadTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a read table
	string createReadTableSQL = "CREATE TABLE reads ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createReadTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing statement table (if any)
	string dropStatementTableSQL = "DROP TABLE IF EXISTS statements";
	sqlite3_exec(dbConnection, dropStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a statement table
	string createStatementTableSQL = "CREATE TABLE statements ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing constant table (if any)
	string dropConstantTableSQL = "DROP TABLE IF EXISTS constants";
	sqlite3_exec(dbConnection, dropConstantTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a constant table
	string createConstantTableSQL = "CREATE TABLE constants ( constantName VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createConstantTableSQL.c_str(), NULL, 0, &errorMessage);


	// drop the existing while table (if any)
	string dropWhileTableSQL = "DROP TABLE IF EXISTS whiles";
	sqlite3_exec(dbConnection, dropWhileTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a while table
	string createWhileTableSQL = "CREATE TABLE whiles ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createWhileTableSQL.c_str(), NULL, 0, &errorMessage);


	// drop the existing if table (if any)
	string dropIfTableSQL = "DROP TABLE IF EXISTS ifs";
	sqlite3_exec(dbConnection, dropIfTableSQL.c_str(), NULL, 0, &errorMessage);

	// create an if table
	string createIfTableSQL = "CREATE TABLE ifs ( line VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createIfTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing parent table (if any)
	string dropParentTableSQL = "DROP TABLE IF EXISTS parents";
	sqlite3_exec(dbConnection, dropParentTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a parent table
	string createParentTableSQL = "CREATE TABLE parents ( parentLine VARCHAR(255) , childLine VARCHAR(255) );";
	sqlite3_exec(dbConnection, createParentTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing grandparent table (if any)
	string dropGrandparentTableSQL = "DROP TABLE IF EXISTS parentst";
	sqlite3_exec(dbConnection, dropGrandparentTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a grandparent table
	string createGrandparentTableSQL = "CREATE TABLE parentst ( grandparentLine VARCHAR(255) , childLine VARCHAR(255) );";
	sqlite3_exec(dbConnection, createGrandparentTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing modifies table (if any)
	string dropModifiesTableSQL = "DROP TABLE IF EXISTS modifies";
	sqlite3_exec(dbConnection, dropModifiesTableSQL.c_str(), NULL, 0, &errorMessage);

	// create modifies table
	string createModifiesTableSQL = "CREATE TABLE modifies ( modifiesLine VARCHAR(255) , variableN VARCHAR(255) , UNIQUE(modifiesLine,variableN) );";
	sqlite3_exec(dbConnection, createModifiesTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing modifiesproc table (if any)
	string dropModifiesprocTableSQL = "DROP TABLE IF EXISTS modifiesproc";
	sqlite3_exec(dbConnection, dropModifiesprocTableSQL.c_str(), NULL, 0, &errorMessage);

	// create modifiesproc table
	string createModifiesprocTableSQL = "CREATE TABLE modifiesproc ( modifiesProc VARCHAR(255) , variableN VARCHAR(255) , UNIQUE(modifiesProc,variableN) );";
	sqlite3_exec(dbConnection, createModifiesprocTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing uses table (if any)
	string dropMusesTableSQL = "DROP TABLE IF EXISTS muses";
	sqlite3_exec(dbConnection, dropMusesTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing uses table (if any)
	string dropUsesTableSQL = "DROP TABLE IF EXISTS uses";
	sqlite3_exec(dbConnection, dropUsesTableSQL.c_str(), NULL, 0, &errorMessage);

	// create uses table
	string createUsesTableSQL = "CREATE TABLE uses ( usesLine VARCHAR(255) , variableN VARCHAR(255) , UNIQUE(usesLine,variableN) );";
	sqlite3_exec(dbConnection, createUsesTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing usesproc table (if any)
	string dropUsesprocTableSQL = "DROP TABLE IF EXISTS usesproc";
	sqlite3_exec(dbConnection, dropUsesprocTableSQL.c_str(), NULL, 0, &errorMessage);

	// create usesproc table
	string createUsesprocTableSQL = "CREATE TABLE usesproc ( usesProc VARCHAR(255) , variableN VARCHAR(255) , UNIQUE(usesProc,variableN) );";
	sqlite3_exec(dbConnection, createUsesprocTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing calls table (if any)
	string dropCallsTableSQL = "DROP TABLE IF EXISTS calls";
	sqlite3_exec(dbConnection, dropCallsTableSQL.c_str(), NULL, 0, &errorMessage);

	// create calls table
	string createCallsTableSQL = "CREATE TABLE calls ( proc1 VARCHAR(255) , proc2 VARCHAR(255) );";
	sqlite3_exec(dbConnection, createCallsTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing callst table (if any)
	string dropCallstTableSQL = "DROP TABLE IF EXISTS callst";
	sqlite3_exec(dbConnection, dropCallstTableSQL.c_str(), NULL, 0, &errorMessage);

	// create callst table
	string createCallstTableSQL = "CREATE TABLE callst ( proc1 VARCHAR(255) , proc2 VARCHAR(255) , UNIQUE(proc1,proc2) );";
	sqlite3_exec(dbConnection, createCallstTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing nexts table (if any)
	string dropNextsTableSQL = "DROP TABLE IF EXISTS nexts";
	sqlite3_exec(dbConnection, dropNextsTableSQL.c_str(), NULL, 0, &errorMessage);

	// create nexts table
	string createNextsTableSQL = "CREATE TABLE nexts ( stmt1 VARCHAR(255) , stmt2 VARCHAR(255) );";
	sqlite3_exec(dbConnection, createNextsTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing nextst table (if any)
	string dropNextstTableSQL = "DROP TABLE IF EXISTS nextst";
	sqlite3_exec(dbConnection, dropNextstTableSQL.c_str(), NULL, 0, &errorMessage);

	// create callst table
	string createNextstTableSQL = "CREATE TABLE nextst ( stmt1 VARCHAR(255) , stmt2 VARCHAR(255) , UNIQUE(stmt1,stmt2));";
	sqlite3_exec(dbConnection, createNextstTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing procstmt table (if any)
	string dropProcstmtTableSQL = "DROP TABLE IF EXISTS procstmt";
	sqlite3_exec(dbConnection, dropProcstmtTableSQL.c_str(), NULL, 0, &errorMessage);

	// create procstmt table
	string createProcstmtTableSQL = "CREATE TABLE procstmt ( proc VARCHAR(255) , stmt VARCHAR(255) );";
	sqlite3_exec(dbConnection, createProcstmtTableSQL.c_str(), NULL, 0, &errorMessage);

	// initialize the result vector
	dbResults = vector<vector<string>>();
}

// method to close the database connection
void Database::close() {
	sqlite3_close(dbConnection);
}

// method to insert a procedure into the database
void Database::insertProcedure(string procedureName) {
	string insertProcedureSQL = "INSERT INTO procedures ('procedureName') VALUES ('" + procedureName + "');";
	sqlite3_exec(dbConnection, insertProcedureSQL.c_str(), NULL, 0, &errorMessage);
}


// method to insert a variable into the database
void Database::insertVariable(string variableName) {
	string insertVariableSQL = "INSERT INTO variables ('variableName') VALUES ('" + variableName + "');";
	sqlite3_exec(dbConnection, insertVariableSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert an assignment into the database
void Database::insertAssignment(string assignmentLine, string lhs, string rhs) {
	string insertAssignmentSQL = "INSERT INTO assignments ('line' , 'lhs' , 'rhs') VALUES ('" + assignmentLine + "' , '" + lhs + "' , '" + rhs + "'); ";
	sqlite3_exec(dbConnection, insertAssignmentSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a print into the database
void Database::insertPrint(string printLine) {
	string insertPrintSQL = "INSERT INTO prints ('line') VALUES ('" + printLine + "');";
	sqlite3_exec(dbConnection, insertPrintSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a read into the database
void Database::insertRead(string readLine) {
	string insertReadSQL = "INSERT INTO reads ('line') VALUES ('" + readLine + "');";
	sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a statement into the database
void Database::insertStatement(string statementLine) {
	string insertStatementSQL = "INSERT INTO statements ('line') VALUES ('" + statementLine + "');";
	sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a constant into the database
void Database::insertConstant(string constantName) {
	string insertConstantSQL = "INSERT INTO constants ('constantName') VALUES ('" + constantName + "');";
	sqlite3_exec(dbConnection, insertConstantSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a while into the database
void Database::insertWhile(string whileLine) {
	string insertWhileSQL = "INSERT INTO whiles ('line') VALUES ('" + whileLine + "');";
	sqlite3_exec(dbConnection, insertWhileSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert an if into the database
void Database::insertIf(string ifLine) {
	string insertIfSQL = "INSERT INTO ifs ('line') VALUES ('" + ifLine + "');";
	sqlite3_exec(dbConnection, insertIfSQL.c_str(), NULL, 0, &errorMessage);
}

//// method to insert a parent into the database
//void Database::insertParent(string parentLine) {
//	string insertParentSQL = "INSERT INTO parents ('parentLine') VALUES ('" + parentLine + "');";
//	sqlite3_exec(dbConnection, insertParentSQL.c_str(), NULL, 0, &errorMessage);
//}

// method to insert a child into the parents table
void Database::insertChild(string parentLine, string childLine) {
	string insertChildSQL = "INSERT INTO parents ('parentLine', 'childLine') VALUES (" + parentLine + ", " + childLine + "); ";
	sqlite3_exec(dbConnection, insertChildSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert a grandchild into the parents table
void Database::insertGrandchild(string grandparentLine, string childLine) {
	string insertGrandchildSQL = "INSERT INTO parentst ('grandparentLine', 'childLine') VALUES (" + grandparentLine + ", " + childLine + "); ";
	sqlite3_exec(dbConnection, insertGrandchildSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the modifies table
void Database::insertModifies(string modifiesLine, string variableN) {
	string insertModifiesSQL = "INSERT INTO modifies ('modifiesLine', 'variableN') VALUES ( '" + modifiesLine + "', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertModifiesSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the modifiesproc table
void Database::insertModifiesproc(string modifiesProc, string variableN) {
	string insertModifiesprocSQL = "INSERT INTO modifiesproc ('modifiesProc', 'variableN') VALUES ( '" + modifiesProc + "', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertModifiesprocSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the uses table
void Database::insertUses(string usesLine, string variableN) {
	string insertUsesSQL = "INSERT INTO uses ('usesLine', 'variableN') VALUES ( '" + usesLine + "', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertUsesSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the usesproc table
void Database::insertUsesproc(string usesProc, string variableN) {
	string insertUsesprocSQL = "INSERT INTO usesproc ('usesProc', 'variableN') VALUES ( '" + usesProc + "', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertUsesprocSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the calls table
void Database::insertCalls(string proc1, string proc2) {
	string insertCallsSQL = "INSERT INTO calls ('proc1', 'proc2') VALUES ( '" + proc1 + "', '" + proc2 + "' ); ";
	sqlite3_exec(dbConnection, insertCallsSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the callst table
void Database::insertCallst(string proc1, string proc2) {
	string insertCallstSQL = "INSERT INTO callst ('proc1', 'proc2') VALUES ( '" + proc1 + "', '" + proc2 + "' ); ";
	sqlite3_exec(dbConnection, insertCallstSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the nexts table
void Database::insertNexts(string stmt1, string stmt2) {
	string insertNextsSQL = "INSERT INTO nexts ('stmt1', 'stmt2') VALUES ( '" + stmt1 + "', '" + stmt2 + "' ); ";
	sqlite3_exec(dbConnection, insertNextsSQL.c_str(), NULL, 0, &errorMessage);

}

// method to insert into the nextst table
void Database::insertNextst(string stmt1, string stmt2) {
	string insertNextstSQL = "INSERT INTO nextst ('stmt1', 'stmt2') VALUES ( '" + stmt1 + "', '" + stmt2 + "' ); ";
	sqlite3_exec(dbConnection, insertNextstSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the procstmt table
void Database::insertProcstmt(string proc, string stmt) {
	string insertProcstmtSQL = "INSERT INTO procstmt ('proc', 'stmt') VALUES ( '" + proc + "', '" + stmt + "' ); ";
	sqlite3_exec(dbConnection, insertProcstmtSQL.c_str(), NULL, 0, &errorMessage);
}

// method to get all the procedures from the database
void Database::getProcedure(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getProceduresSQL = "SELECT * FROM procedures;";
	sqlite3_exec(dbConnection, getProceduresSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the constants from the database
void Database::getConstant(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getConstantSQL = "SELECT * FROM constants;";
	sqlite3_exec(dbConnection, getConstantSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the variables from the database
void Database::getVariable(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getVariableSQL = "SELECT * FROM variables;";
	sqlite3_exec(dbConnection, getVariableSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the statements from the database (all except procedure and variable)
void Database::getStatement(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getStatementSQL = "SELECT * FROM statements;";
	sqlite3_exec(dbConnection, getStatementSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the read statements from the database
void Database::getRead(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getReadSQL = "SELECT * FROM reads;";
	sqlite3_exec(dbConnection, getReadSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the print statements from the database
void Database::getPrint(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getPrintSQL = "SELECT * FROM prints;";
	sqlite3_exec(dbConnection, getPrintSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the while statements from the database
void Database::getWhile(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getPrintSQL = "SELECT * FROM whiles;";
	sqlite3_exec(dbConnection, getPrintSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the if statements from the database
void Database::getIf(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getPrintSQL = "SELECT * FROM ifs;";
	sqlite3_exec(dbConnection, getPrintSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}


// method to get all the assignment statements from the database
void Database::getAssignment(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getAssignmentSQL = "SELECT line FROM assignments;";
	sqlite3_exec(dbConnection, getAssignmentSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get all the assignment statement patterns from the database
void Database::getAssignmentPattern(vector<vector<string>>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getAssignmentSQL = "SELECT * FROM assignments;";
	sqlite3_exec(dbConnection, getAssignmentSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

// method to get all the parents from the database
void Database::getParent(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue) {
	// clear the existing results
	dbResults.clear();
	string getParentSQL;

	if (parentType == "undeclared") {
		if (childType == "undeclared") {
			getParentSQL = "SELECT parentLine, childLine FROM parents;";
		}
		else if (childType == "line number") {
			getParentSQL = "SELECT parentLine, childLine from parents WHERE childLine = " + childValue + ";";
		}
		else {  // child is synonym
			getParentSQL = "SELECT parentLine, childLine FROM parents WHERE childLine IN (SELECT line FROM " + convertToDbName(childType) + ");";
		}
	}
	else if (parentType == "line number") {
		if (childType == "undeclared") {
			getParentSQL = "SELECT parentLine, childLine FROM parents WHERE parentLine = " + parentValue + ";";
		}
		else if (childType == "line number") {
			getParentSQL = "SELECT parentLine, childLine from parents WHERE parentLine = " + parentValue + " AND childLine = " + childValue + ";";
		}
		else { // child is synonym
			getParentSQL = "SELECT parentLine, childLine FROM parents WHERE parentLine = " + parentValue + " AND childLine IN (SELECT line FROM " + convertToDbName(childType) + "); ";
		}
	}
	else { //parent is a synonym
		if (childType == "undeclared") {
			getParentSQL = "SELECT parentLine, childLine FROM parents WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ");";
		}
		else if (childType == "line number") {
			getParentSQL = "SELECT parentLine, childLine from parents WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ") AND childLine = " + childValue + ";";
		}
		else {  // child is synonym
			getParentSQL = "SELECT parentLine, childLine FROM parents WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ") AND childLine IN (SELECT line FROM " + convertToDbName(childType) + "); ";
		}
	}

	sqlite3_exec(dbConnection, getParentSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getParentT(vector<vector<string>>& results, string parentType, string parentValue, string childType, string childValue)
{
	// clear the existing results
	dbResults.clear();
	string getParentTSQL = "SELECT parentLine, childLine FROM ( SELECT * FROM parents UNION SELECT * FROM parentst )";


	if (parentType == "undeclared") {
		// child is undeclared not written because it just returns the current "temporary" union table
		if (childType == "line number") {
			getParentTSQL += " WHERE childLine = " + childValue + "; ";
		}
		else { // child is synonym
			getParentTSQL += " WHERE childLine IN (SELECT line FROM " + convertToDbName(childType) + ");";
		}
	}
	else if (parentType == "line number") {
		if (childType == "undeclared") {
			getParentTSQL += " WHERE parentLine = " + parentValue + ";";
		}
		else if (childType == "line number") {
			getParentTSQL += " WHERE parentLine = " + parentValue + " AND childLine = " + childValue + ";";
		}
		else { // child is synonym
			getParentTSQL += " WHERE parentLine = " + parentValue + " AND childLine IN (SELECT line FROM " + convertToDbName(childType) + ");";
		}
	}
	else { //parent is a synonym
		if (childType == "undeclared") {
			getParentTSQL += " WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ");";
		}
		else if (childType == "line number") {
			getParentTSQL += " WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ") AND childLine = " + childValue + ";";
		}
		else { // child is synonym
			getParentTSQL += " WHERE parentLine IN (SELECT line FROM " + convertToDbName(parentType) + ") AND childLine IN (SELECT line FROM " + convertToDbName(childType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getParentTSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getModifies(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	// clear the existing results
	dbResults.clear();
	string getModifiesSQL;

	// The first argument for Modifies and Uses cannot be �_� because it is unclear whether the �_� stands for a statement or a procedure.

	if (firstArgumentType == "line number") {
		if (secondArgumentType == "IDENT") {
			getModifiesSQL = "SELECT modifiesLine, variableN FROM modifies WHERE modifiesLine = " + firstArgumentValue + "AND variableN = \"" + secondArgumentValue + "\";";
		}
		else { // secondArgumentType is a _ OR synonym i.e. variable design entity
			getModifiesSQL = "SELECT modifiesLine, variableN FROM modifies WHERE modifiesLine = " + firstArgumentValue + ";";
		}
	}
	else {

		if (secondArgumentType == "IDENT") {
			getModifiesSQL = "SELECT modifiesLine, variableN FROM modifies WHERE variableN = \"" + secondArgumentValue + "\" AND modifiesLine IN (SELECT line FROM " + convertToDbName(firstArgumentType) + "); ";
		}
		else {  // secondArgumentType is a _ OR synonym i.e. variable design entity
			getModifiesSQL = "SELECT modifiesLine, variableN FROM modifies WHERE modifiesLine IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getModifiesSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getModifiesP(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	// clear the existing results
	dbResults.clear();
	string getModifiesPSQL;

	// The first argument for Modifies and Uses cannot be �_� because it is unclear whether the �_� stands for a statement or a procedure.

	if (firstArgumentType == "IDENT") {
		if (secondArgumentType == "IDENT") {
			getModifiesPSQL = "SELECT modifiesProc, variableN FROM modifiesproc WHERE modifiesProc = \"" + firstArgumentValue + "\" AND variableN = \"" + secondArgumentValue + "\";";
		}
		else { // secondArgumentType is a _ OR synonym i.e. variable design entity
			getModifiesPSQL = "SELECT modifiesProc, variableN FROM modifiesproc WHERE modifiesProc = \"" + firstArgumentValue + "\";";
		}
	}
	else { //synonym
		string tableName = convertToDbName(firstArgumentType);
		string columnName = tableName == "calls" ? "proc1" : "procedureName";
		if (secondArgumentType == "IDENT") {
			getModifiesPSQL = "SELECT modifiesProc, variableN FROM modifiesproc WHERE variableN = \"" + secondArgumentValue + "\" AND modifiesProc IN (SELECT " + columnName + " FROM " + convertToDbName(firstArgumentType) + ");";
		}
		else {  // secondArgumentType is a _ OR synonym i.e. variable design entity
			getModifiesPSQL = "SELECT modifiesProc, variableN FROM modifiesproc WHERE modifiesProc IN (SELECT " + columnName + " FROM " + convertToDbName(firstArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getModifiesPSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

// method to get variableN from the database modifies for specific modifiesLine
void Database::getCallsTmodifies(vector<string>& results, string callee) {
	// clear the existing results
	dbResults.clear();

	string getCallsTmodifiesSQL = "SELECT variableN FROM modifiesproc WHERE modifiesProc = '" + callee + "';";
	sqlite3_exec(dbConnection, getCallsTmodifiesSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

void Database::getUses(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	// clear the existing results
	dbResults.clear();
	string getUsesSQL;

	// The first argument for Modifies and Uses cannot be �_� because it is unclear whether the �_� stands for a statement or a procedure.

	if (firstArgumentType == "line number") {
		if (secondArgumentType == "IDENT") {
			getUsesSQL = "SELECT usesLine, variableN FROM uses WHERE usesLine = " + firstArgumentValue + "AND variablenN = \"" + secondArgumentValue + "\";";
		}
		else { // secondArgumentType is a _ OR synonym i.e. variable design entity
			getUsesSQL = "SELECT usesLine, variableN FROM uses WHERE usesLine = " + firstArgumentValue + ";";
		}
	}
	else {
		if (secondArgumentType == "IDENT") {
			getUsesSQL = "SELECT usesLine, variableN FROM uses WHERE variableN = \"" + secondArgumentValue + "\" AND usesLine IN (SELECT line FROM " + convertToDbName(firstArgumentType) + "); ";
		}
		else {  // secondArgumentType is a _ OR synonym i.e. variable design entity
			getUsesSQL = "SELECT usesLine, variableN FROM uses WHERE usesLine IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getUsesSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getUsesP(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	// clear the existing results
	dbResults.clear();
	string getUsesPSQL;

	// The first argument for Modifies and Uses cannot be �_� because it is unclear whether the �_� stands for a statement or a procedure.

	if (firstArgumentType == "IDENT") {
		if (secondArgumentType == "IDENT") {
			getUsesPSQL = "SELECT usesProc, variableN FROM usesproc WHERE usesProc = \"" + firstArgumentValue + "\" AND variableN = \"" + secondArgumentValue + "\";";
		}
		else { // secondArgumentType is a _ OR synonym i.e. variable design entity
			getUsesPSQL = "SELECT usesProc, variableN FROM usesproc WHERE usesProc = \"" + firstArgumentValue + "\";";
		}
	}
	else { //synonym
		string tableName = convertToDbName(firstArgumentType);
		string columnName = tableName == "calls" ? "proc1" : "procedureName";
		if (secondArgumentType == "IDENT") {
			getUsesPSQL = "SELECT usesProc, variableN FROM usesproc WHERE variableN = \"" + secondArgumentValue + "\" AND usesProc IN (SELECT " + columnName + " FROM " + convertToDbName(firstArgumentType) + ");";
		}
		else {  // secondArgumentType is a _ OR synonym i.e. variable design entity
			getUsesPSQL = "SELECT usesProc, variableN FROM usesproc WHERE usesProc IN (SELECT " + columnName + " FROM " + convertToDbName(firstArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getUsesPSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getNext(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	dbResults.clear();
	string getNextSQL;

	if (firstArgumentType == "undeclared") {
		if (secondArgumentType == "undeclared") {
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts;";
		}
		else if (secondArgumentType == "line number") {
			getNextSQL = "SELECT stmt1, stmt2 from nexts WHERE stmt2 = " + secondArgumentValue + ";";
		}
		else { 
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts WHERE stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}
	else if (firstArgumentType == "line number") {
		if (secondArgumentType == "undeclared") {
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts WHERE stmt1 = " + firstArgumentValue + ";";
		}
		else if (secondArgumentType == "line number") {
			getNextSQL = "SELECT stmt1, stmt2 from nexts WHERE stmt1 = " + firstArgumentValue + " AND stmt2 = " + secondArgumentValue + ";";
		}
		else { 
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts WHERE stmt1 = " + firstArgumentValue + " AND stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + "); ";
		}
	}
	else { //parent is a synonym
		if (secondArgumentType == "undeclared") {
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ");";
		}
		else if (secondArgumentType == "line number") {
			getNextSQL = "SELECT stmt1, stmt2 from nexts WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ") AND stmt2 = " + secondArgumentValue + ";";
		}
		else {  
			getNextSQL = "SELECT stmt1, stmt2 FROM nexts WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ") AND stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + "); ";
		}
	}

	sqlite3_exec(dbConnection, getNextSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getNextT(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	dbResults.clear();
	string getNextTSQL = "SELECT stmt1, stmt2 FROM nextst";


	if (firstArgumentType == "undeclared") {
		// child is undeclared not written because it just returns the current "temporary" union table
		if (secondArgumentType == "line number") {
			getNextTSQL += " WHERE stmt2 = " + secondArgumentValue + "; ";
		}
		else { // child is synonym
			getNextTSQL += " WHERE stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}
	else if (firstArgumentType == "line number") {
		if (secondArgumentType == "undeclared") {
			getNextTSQL += " WHERE stmt1 = " + firstArgumentValue + ";";
		}
		else if (secondArgumentType == "line number") {
			getNextTSQL += " WHERE stmt1 = " + firstArgumentValue + " AND stmt2 = " + secondArgumentValue + ";";
		}
		else { // child is synonym
			getNextTSQL += " WHERE stmt1 = " + firstArgumentValue + " AND stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}
	else { //parent is a synonym
		if (secondArgumentType == "undeclared") {
			getNextTSQL += " WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ");";
		}
		else if (secondArgumentType == "line number") {
			getNextTSQL += " WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ") AND stmt2 = " + secondArgumentValue + ";";
		}
		else { // child is synonym
			getNextTSQL += " WHERE stmt1 IN (SELECT line FROM " + convertToDbName(firstArgumentType) + ") AND stmt2 IN (SELECT line FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getNextTSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}
}

void Database::getCalls(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	dbResults.clear();
	string getCallSQL;

	if (firstArgumentType == "undeclared") {
		if (secondArgumentType == "undeclared") {
			getCallSQL = "SELECT proc1, proc2 FROM calls;";
		}
		else if (secondArgumentType == "IDENT") {
			getCallSQL = "SELECT proc1, proc2 from calls WHERE proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallSQL = "SELECT proc1, proc2 FROM calls WHERE proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}
		
	}
	else if (firstArgumentType == "IDENT") {
		if (secondArgumentType == "undeclared") {
			getCallSQL = "SELECT proc1, proc2 FROM calls WHERE proc1 = \"" + firstArgumentValue + "\";";
		}
		else if (secondArgumentType == "IDENT") {
			getCallSQL = "SELECT proc1, proc2 from calls WHERE proc1 = \"" + firstArgumentValue + "\" AND proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallSQL = "SELECT proc1, proc2 FROM calls WHERE proc1 = \"" + firstArgumentValue + "\" AND proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + "); ";
		}
	}
	else { //parent is a synonym
		if (secondArgumentType == "undeclared") {
			getCallSQL = "SELECT proc1, proc2 FROM calls WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}
		else if (secondArgumentType == "IDENT") {
			getCallSQL = "SELECT proc1, proc2 from calls WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ") AND proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallSQL = "SELECT proc1, proc2 FROM calls WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ") AND proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getCallSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}

}

void Database::getCallsT(vector<vector<string>>& results, string firstArgumentType, string firstArgumentValue, string secondArgumentType, string secondArgumentValue)
{
	dbResults.clear();
	string getCallTSQL;

	if (firstArgumentType == "undeclared") {
		if (secondArgumentType == "undeclared") {
			getCallTSQL = "SELECT proc1, proc2 FROM callst;";
		}
		else if (secondArgumentType == "IDENT") {
			getCallTSQL = "SELECT proc1, proc2 from callst WHERE proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallTSQL = "SELECT proc1, proc2 FROM callst WHERE proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}

	}
	else if (firstArgumentType == "IDENT") {
		if (secondArgumentType == "undeclared") {
			getCallTSQL = "SELECT proc1, proc2 FROM callst WHERE proc1 = \"" + firstArgumentValue + "\";";
		}
		else if (secondArgumentType == "IDENT") {
			getCallTSQL = "SELECT proc1, proc2 from callst WHERE proc1 = \"" + firstArgumentValue + "\" AND proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallTSQL = "SELECT proc1, proc2 FROM callst WHERE proc1 = \"" + firstArgumentValue + "\" AND proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + "); ";
		}
	}
	else { //parent is a synonym
		if (secondArgumentType == "undeclared") {
			getCallTSQL = "SELECT proc1, proc2 FROM callst WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}
		else if (secondArgumentType == "IDENT") {
			getCallTSQL = "SELECT proc1, proc2 from callst WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ") AND proc2 = \"" + secondArgumentValue + "\";";
		}
		else {
			getCallTSQL = "SELECT proc1, proc2 FROM callst WHERE proc1 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ") AND proc2 IN (SELECT procedureName FROM " + convertToDbName(secondArgumentType) + ");";
		}
	}

	sqlite3_exec(dbConnection, getCallTSQL.c_str(), callback, 0, &errorMessage);

	for (vector<string> dbRow : dbResults) {
		results.push_back(dbRow);
	}

}

// method to get variableN from the database modifies for specific usesLine
void Database::getCallsTuses(vector<string>& results, string callee) {
	// clear the existing results
	dbResults.clear();

	string getCallsTusesSQL = "SELECT variableN FROM usesproc WHERE usesProc = '" + callee + "';";
	sqlite3_exec(dbConnection, getCallsTusesSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
}

// method to get procedure from the database procstmt for specific statement line
void Database::getProcstmt(vector<string>& results, string stmt) {
	// clear the existing results
	dbResults.clear();

	string getProcstmtSQL = "SELECT proc FROM procstmt WHERE stmt = '" + stmt + "';";
	sqlite3_exec(dbConnection, getProcstmtSQL.c_str(), callback, 0, &errorMessage);

	// postprocess the results from the database so that the output is just a vector of procedure names
	for (vector<string> dbRow : dbResults) {
		string result;
		result = dbRow.at(0);
		results.push_back(result);
	}
	
}


// callback method to put one row of results from the database into the dbResults vector
// This method is called each time a row of results is returned from the database
int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
	NotUsed = 0;
	vector<string> dbRow;

	// argc is the number of columns for this row of results
	// argv contains the values for the columns
	// Each value is pushed into a vector.
	for (int i = 0; i < argc; i++) {
		dbRow.push_back(argv[i]);
	}

	// The row is pushed to the vector for storing all rows of results 
	dbResults.push_back(dbRow);

	return 0;
}

string Database::convertToDbName(string designEntity)
{
	if (designEntity == "stmt") {
		return "statements";
	}
	else if (designEntity == "assign") {
		return "assignments";
	}
	else {
		return designEntity + "s";
	}
}
