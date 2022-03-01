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
	string createAssignmentTableSQL = "CREATE TABLE assignments ( assignmentLine VARCHAR(255) , lhs VARCHAR(255) , rhs VARCHAR(255));";
	sqlite3_exec(dbConnection, createAssignmentTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing print table (if any)
	string dropPrintTableSQL = "DROP TABLE IF EXISTS prints";
	sqlite3_exec(dbConnection, dropPrintTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a print table
	string createPrintTableSQL = "CREATE TABLE prints ( printLine VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createPrintTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing read table (if any)
	string dropReadTableSQL = "DROP TABLE IF EXISTS reads";
	sqlite3_exec(dbConnection, dropReadTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a read table
	string createReadTableSQL = "CREATE TABLE reads ( readLine VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createReadTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing statement table (if any)
	string dropStatementTableSQL = "DROP TABLE IF EXISTS statements";
	sqlite3_exec(dbConnection, dropStatementTableSQL.c_str(), NULL, 0, &errorMessage);

	// create a statement table
	string createStatementTableSQL = "CREATE TABLE statements ( statementLine VARCHAR(255) PRIMARY KEY);";
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
	string createWhileTableSQL = "CREATE TABLE whiles ( whileLine VARCHAR(255) PRIMARY KEY);";
	sqlite3_exec(dbConnection, createWhileTableSQL.c_str(), NULL, 0, &errorMessage);


	// drop the existing if table (if any)
	string dropIfTableSQL = "DROP TABLE IF EXISTS ifs";
	sqlite3_exec(dbConnection, dropIfTableSQL.c_str(), NULL, 0, &errorMessage);

	// create an if table
	string createIfTableSQL = "CREATE TABLE ifs ( ifLine VARCHAR(255) PRIMARY KEY);";
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
	string createModifiesTableSQL = "CREATE TABLE modifies ( modifiesLine VARCHAR(255) , variableN VARCHAR(255) );";
	sqlite3_exec(dbConnection, createModifiesTableSQL.c_str(), NULL, 0, &errorMessage);

	// drop the existing uses table (if any)
	string dropUsesTableSQL = "DROP TABLE IF EXISTS uses";
	sqlite3_exec(dbConnection, dropUsesTableSQL.c_str(), NULL, 0, &errorMessage);

	// create uses table
	string createUsesTableSQL = "CREATE TABLE uses ( usesLine VARCHAR(255) , variableN VARCHAR(255) );";
	sqlite3_exec(dbConnection, createUsesTableSQL.c_str(), NULL, 0, &errorMessage);

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
	string insertAssignmentSQL = "INSERT INTO assignments ('assignmentLine' , 'lhs' , 'rhs') VALUES ('" + assignmentLine + "' , '" + lhs + "' , '" + rhs +"'); ";
	sqlite3_exec(dbConnection, insertAssignmentSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a print into the database
void Database::insertPrint(string printLine) {
	string insertPrintSQL = "INSERT INTO prints ('printLine') VALUES ('" + printLine + "');";
	sqlite3_exec(dbConnection, insertPrintSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a read into the database
void Database::insertRead(string readLine) {
	string insertReadSQL = "INSERT INTO reads ('readLine') VALUES ('" + readLine + "');";
	sqlite3_exec(dbConnection, insertReadSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a statement into the database
void Database::insertStatement(string statementLine) {
	string insertStatementSQL = "INSERT INTO statements ('statementLine') VALUES ('" + statementLine + "');";
	sqlite3_exec(dbConnection, insertStatementSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a constant into the database
void Database::insertConstant(string constantName) {
	string insertConstantSQL = "INSERT INTO constants ('constantName') VALUES ('" + constantName + "');";
	sqlite3_exec(dbConnection, insertConstantSQL.c_str(), NULL, 0, &errorMessage);
}



// method to insert a while into the database
void Database::insertWhile(string whileLine) {
	string insertWhileSQL = "INSERT INTO whiles ('whileLine') VALUES ('" + whileLine + "');";
	sqlite3_exec(dbConnection, insertWhileSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert an if into the database
void Database::insertIf(string ifLine) {
	string insertIfSQL = "INSERT INTO ifs ('ifLine') VALUES ('" + ifLine + "');";
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
	string insertModifiesSQL = "INSERT INTO modifies ('modifiesLine', 'variableN') VALUES ( '"+ modifiesLine +"', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertModifiesSQL.c_str(), NULL, 0, &errorMessage);
}

// method to insert into the uses table
void Database::insertUses(string usesLine, string variableN) {
	string insertUsesSQL = "INSERT INTO uses ('usesLine', 'variableN') VALUES ( '" + usesLine + "', '" + variableN + "' ); ";
	sqlite3_exec(dbConnection, insertUsesSQL.c_str(), NULL, 0, &errorMessage);
}




// method to get all the procedures from the database
void Database::getProcedure(vector<string>& results){
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

// method to get all the assignment statements from the database
void Database::getAssignment(vector<string>& results) {
	// clear the existing results
	dbResults.clear();

	// retrieve the procedures from the procedure table
	// The callback method is only used when there are results to be returned.
	string getAssignmentSQL = "SELECT * FROM assignments;";
	sqlite3_exec(dbConnection, getAssignmentSQL.c_str(), callback, 0, &errorMessage);

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
