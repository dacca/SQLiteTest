// SQLiteTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "sqlite3.h"

#include <stdio.h>
#include <sstream>
#include <time.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
//
//:ag tmp test code
//
struct RPTime {
	RPTime(size_t subtract_hours = 1) { hours_ago(subtract_hours); }
	void minutes_ago(size_t subtract_minutes) {
		time_t tt;
		tt = time(&tt);
		tt -= subtract_minutes * 60; // subtract n minutes
		_val = tt * 1000000; // convert to RPtime, which is microseconds
	}
	void hours_ago(size_t subtract_hours) {
		minutes_ago(subtract_hours * 60);
	}
	uint64_t _val;
};
ostream& operator<<(ostream& os, RPTime rp) {
	return os << rp._val; 
}

int main(int argc, char **argv) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	/*
	if(argc != 3) {
		fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
		return(1);
	}
	*/
	// const char* dbfile = "d:/ag/code/cpp/SQLiteTest/SQLiteTest/cookies.sqlite";
	const char* dbfile = "c:/Users/ag/AppData/Roaming/Mozilla/Firefox/Profiles/4igdw207.default/places.sqlite";
	//const char* sql_statement = "SELECT * from moz_places where typed = 1";
	const char* SQL_1 = "SELECT datetime(moz_historyvisits.visit_date/1000000,'unixepoch'), moz_places.url \
		FROM moz_places, moz_historyvisits \
		WHERE moz_places.id = moz_historyvisits.place_id \
		AND moz_places.url LIKE \'%ljudbojen%\'"; // moz_historyvisits.visit_date > ";
	const char* SQL_2 = "SELECT moz_historyvisits.visit_date/1000000, moz_places.url \
		FROM moz_places, moz_historyvisits \
		WHERE (moz_places.url LIKE \'%ljudbojen%\' OR moz_places.url LIKE \'%.ericsson.%\') \
		AND(moz_historyvisits.visit_date > 1455697538 * 1000000 AND moz_historyvisits.visit_date < 1455712950 * 1000000) \
		AND moz_places.id = moz_historyvisits.place_id \
		ORDER BY moz_historyvisits.visit_date"; // moz_historyvisits.visit_date > ";
	rc = sqlite3_open(dbfile, &db); // argv[1], &db);
	if(rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return(1);
	}
	stringstream ss;
	RPTime rptime(5);
	ss << SQL_2 << endl; // rptime << endl;

	rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg); //argv[2], callback, 0, &zErrMsg);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}
