#include "database.h"

Database::Database(string dbpath) {
	initialize(dbpath);
}

void Database::initialize(string dbpath) {
	sqlite3_open(dbpath.c_str(), &database);
}

void Database::close() {
	sqlite3_close(database);
}

string Database::query(string st) {
	char *err;
	string ret = "";

	sqlite3_exec(database, st.c_str(), NULL, NULL, &err);

	if(err != NULL)
		ret = err;
	return ret;
}

string Database::prepared_statement_new(sqlite3_stmt **stmt, string query) {
	char *err;
	string ret = "";

	sqlite3_exec(database, "BEGIN TRANSACTION", NULL, NULL, &err);
	sqlite3_prepare_v2(database, query.c_str(), query.length(), stmt, NULL);

	if(err != NULL)
		ret = err;
	return ret;
}

string Database::prepared_statement_do(sqlite3_stmt **stmt) {
	char *err;
	string ret = "";

	sqlite3_exec(database, "COMMIT TRANSACTION", NULL, NULL, &err);
	sqlite3_finalize(*stmt);

	if(err != NULL)
		ret = err;
	return ret;
}
