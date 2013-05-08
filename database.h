#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <sqlite3.h>
#include <iostream>

using namespace std;

class Database {
private:
	sqlite3 *database;
public:
	Database(string dbpath);
	void initialize(string dbpath);
	void close();
	string query(string st);
	string prepared_statement_new(sqlite3_stmt **stmt, string query);
	string prepared_statement_do(sqlite3_stmt **stmt);
};

#endif // DATABASE_H_INCLUDED
