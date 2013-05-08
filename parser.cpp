#include <boost/spirit/home/classic.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sqlite3.h>
#include "database.h"

using namespace boost::spirit::classic;
using namespace std;

typedef struct {
	string name;
	string prefs;
} dataset;

dataset data;

void db_to_file(sqlite3 *db, string path) {
	ofstream file(path.c_str());
	// TODO
	file.close();
}

template<typename IteratorT>
void _pname(IteratorT begin, IteratorT end) {
    string str(begin, end);
    data.name = str;
}

template<typename IteratorT>
void _prest(IteratorT begin, IteratorT end) {
    string str(begin, end);
    data.prefs = str;
}

int main() {
	string err = "";

	Database dbase("parser.sqlite");
	err += dbase.query("DROP TABLE channels;");
	err += dbase.query("DROP TABLE preferences;");
	err += dbase.query("CREATE TABLE channels (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, prefs TEXT);");
	err += dbase.query("CREATE TABLE preferences (var TEXT PRIMARY KEY, value TEXT);");

	rule<> _CNAME = *(anychar_p - chlit<char>(':'));
	rule<> _CREST = *(anychar_p - eol_p - end_p);
	rule<> _CHANNEL = _CNAME[&_pname<const char*>] >> chlit<char>(':') >> _CREST[&_prest<const char*>];

	string line;
	ifstream f_channels("/home/w33ky/c.conf.M");
	if(f_channels.is_open()) {
		sqlite3_stmt *statement;
		err += dbase.prepared_statement_new(&statement, "INSERT INTO channels VALUES (?1, ?2, ?3)");

		while(f_channels.good()) {
			getline(f_channels, line);

			if(parse(line.c_str(), _CHANNEL).full) {
				cout << "Valid... Pushing " << data.name << " to DB\n";

				sqlite3_bind_text(statement, 2, data.name.c_str(), data.name.size(), SQLITE_TRANSIENT);
				sqlite3_bind_text(statement, 3, data.prefs.c_str(), data.prefs.size(), SQLITE_TRANSIENT);

				if (sqlite3_step(statement) != SQLITE_DONE) {
					printf("Commit Failed!\n");
				}

				sqlite3_reset(statement);
			}
			else {
				cout << "Not valid\n";
			}
		}
		f_channels.close();

		err += dbase.prepared_statement_do(&statement);
	}
	dbase.close();

	cout << err;

	return 0;
}
