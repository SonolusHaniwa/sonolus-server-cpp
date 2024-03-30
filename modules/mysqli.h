#ifndef DATABASE_H
#define DATABASE_H
using namespace std;

#ifdef ENABLE_MYSQL
#include<mysql/mysql.h>
#endif
#ifdef ENABLE_SQLITE
#include<sqlite3.h>
#endif
#ifdef __EMSCRIPTEN__
extern "C" {
	extern char* __builtin_emscripten_query(char* sql);
	extern int __builtin_emscripten_execute(char* sql);
}
#endif

typedef vector<map<string,string> > dbres;

class DB_Controller {
	private:
	#ifdef ENABLE_SQLITE 
	sqlite3 *db;
	#endif

	#ifdef ENABLE_MYSQL
	MYSQL mysqli_connect(const char* host, const char* user, const char* passwd, const char* db, int port) {
		MYSQL mysql, *res1; res1 = mysql_init(&mysql); 
		if (res1 == NULL) {
			writeLog(LOG_LEVEL_ERROR, "Failed to initialize MYSQL structure");
			return MYSQL();
		} bool res2 = mysql_real_connect(&mysql, host, user, passwd, db, port, nullptr, CLIENT_MULTI_STATEMENTS); 
		if (!res2) {
			writeLog(LOG_LEVEL_ERROR, "Failed to connect to MySQL server: " + string(mysql_error(&mysql)));
			return MYSQL();
		} return mysql;
	}
	#endif

	dbres mysqli_query(const char* sql) {
		#ifdef ENABLE_MYSQL
		writeLog(LOG_LEVEL_DEBUG, "Execute database: " + string(sql));
		MYSQL mysql = mysqli_connect(appConfig["mysql.hostname"].asString().c_str(), appConfig["mysql.username"].asString().c_str(), 
			appConfig["mysql.password"].asString().c_str(), appConfig["mysql.database"].asString().c_str(), appConfig["mysql.port"].asInt());
		bool res1 = mysql_query(&mysql, sql);
		if (res1) {
			writeLog(LOG_LEVEL_ERROR, "Failed to query database: " + string(mysql_error(&mysql)));
			return dbres();
		}
		MYSQL_RES* res2 = mysql_store_result(&mysql);
		if (!res2) {
			writeLog(LOG_LEVEL_ERROR, "Failed to query database: " + string(mysql_error(&mysql)));
			return dbres();
		}
		vector<string> field; MYSQL_FIELD* fd; MYSQL_ROW row;
		dbres res; map<string, string> tmp;
		for (int i = 0; fd = mysql_fetch_field(res2); i++) field.push_back(fd->name);
		while (row = mysql_fetch_row(res2)) {
			for (int i = 0; i < field.size(); i++) tmp[field[i]] = row[i] == NULL ? "" : row[i];
			res.push_back(tmp);
		} tmp.clear(); delete[] row;
		mysql_free_result(res2); mysql_close(&mysql);
		return res;
		#else 
		writeLog(LOG_LEVEL_ERROR, "This program doesn't support MySQL/MariaDB server. You need to compile this program with the option `-DENABLE_MYSQL -lmysqlclient`");
		exit(3);
		#endif
	}

	int mysqli_execute(const char* sql) {
		#ifdef ENABLE_MYSQL
		writeLog(LOG_LEVEL_DEBUG, "Execute database: " + string(sql));
		MYSQL mysql = mysqli_connect(appConfig["mysql.hostname"].asString().c_str(), appConfig["mysql.username"].asString().c_str(), 
			appConfig["mysql.password"].asString().c_str(), appConfig["mysql.database"].asString().c_str(), appConfig["mysql.port"].asInt());
		if (mysql_query(&mysql, sql)) {
			writeLog(LOG_LEVEL_ERROR, "Failed to execute database: " + string(mysql_error(&mysql)));
			return 0;
		} int rows = mysql_affected_rows(&mysql);
		mysql_close(&mysql);
		return rows;
		#else 
		writeLog(LOG_LEVEL_ERROR, "This program doesn't support MySQL/MariaDB server. You need to compile this program with the option `-DENABLE_MYSQL -lmysqlclient`");
		exit(3);
		#endif
	}

	dbres sqlite_query(const char* sql) {
		#ifdef ENABLE_SQLITE
		int ret;
		writeLog(LOG_LEVEL_DEBUG, "Execute database: " + string(sql));

        dbres res;
        char** azResult;
        int nrow, ncolumn;
        char* zErrMsg = 0;
        if ((ret = sqlite3_get_table(db, sql, &azResult, &nrow, &ncolumn, &zErrMsg)) != SQLITE_OK) {
            writeLog(LOG_LEVEL_ERROR, "Failed to query database: " + string(zErrMsg));
            sqlite3_free(zErrMsg);
            return dbres();
        } int i = 0, j = 0, index = 0;
        index = ncolumn;
        for (int i = 0; i < nrow; i++) {
            argvar tmp;
            for (int j = 0; j < ncolumn; j++) tmp[azResult[j]] = azResult[index] == NULL ? "" : azResult[index], index++;
            res.push_back(tmp);
        }
        sqlite3_free_table(azResult);
        return res;
		#else 
		writeLog(LOG_LEVEL_ERROR, "This program doesn't support SQLite. You need to compile this program with the option `-DENABLE_SQLITE -lsqlite3`");
		exit(3);
		#endif
	}

	int sqlite_execute(const char* sql) {
		#ifdef ENABLE_SQLITE
		writeLog(LOG_LEVEL_DEBUG, "Execute database: " + string(sql));

        char* zErrMsg;
        int rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if(rc != SQLITE_OK) {
            writeLog(LOG_LEVEL_ERROR, "Failed to execute database: " + string(zErrMsg));
			sqlite3_free(zErrMsg); 
			return 0;
        }
        sqlite3_free(zErrMsg); 
		return sqlite3_changes(db);
		#else 
		writeLog(LOG_LEVEL_ERROR, "This program doesn't support SQLite. You need to compile this program with the option `-DENABLE_SQLITE -lsqlite3`");
		exit(3);
		#endif
    }

	dbres emscripten_query(const char* sql) {
		#ifdef __EMSCRIPTEN__
		writeLog(LOG_LEVEL_DEBUG, "Execute database: " + string(sql));
		char* tmp = __builtin_emscripten_query(const_cast<char*>(sql));
		string result = string(tmp, strlen(tmp)); Json::Value obj;
		json_decode(result, obj); dbres res;
		for (int i = 0; i < obj.size(); i++) {
			map<string, string> tmp;
			Json::Value::Members mem = obj[i].getMemberNames();
			for (auto it = mem.begin(); it != mem.end(); it++) tmp[*it] = obj[i][*it].asString();
			res.push_back(tmp);
		} return res;
		#else 
		writeLog(LOG_LEVEL_ERROR, "You should't use emscripten database in terminal! Maybe you want to compile this program with the command `emcc`?");
		exit(3);
		#endif
	}

	int emscripten_execute(const char* sql) {
		#ifdef __EMSCRIPTEN__
		return 0;
		#else 
		writeLog(LOG_LEVEL_ERROR, "You should't use emscripten database in terminal! Maybe you want to compile this program with the command `emcc`?");
		exit(3);
		#endif

	}

	public:
	
	DB_Controller(){}
	DB_Controller(bool unused){
		#ifdef ENABLE_SQLITE
        int ret = sqlite3_open(appConfig["sqlite.dbfile"].asString().c_str(), &db);
        if (ret) {
            writeLog(LOG_LEVEL_ERROR, "Failed to open SQLite Database: " + string(sqlite3_errmsg(db)));
			return;
        }

		if (appConfig["sqlite.dbfile"].asString() == ":memory:") {
			char* zErrMsg;
			int rc = sqlite3_exec(db, readFile(appConfig["sqlite.sqlfile"].asString()).c_str(), NULL, 0, &zErrMsg);
			if(rc != SQLITE_OK) {
				writeLog(LOG_LEVEL_ERROR, "Failed to execute database: " + string(zErrMsg));
				sqlite3_free(zErrMsg); 
				return;
			}
			sqlite3_free(zErrMsg); 
		}
		#endif
	}

	dbres query(string sql) {
		if (appConfig["database"].asString() == "mysql") return mysqli_query(sql.c_str());
		else if (appConfig["database"].asString() == "sqlite") return sqlite_query(sql.c_str());
		else if (appConfig["database"].asString() == "emscripten") return emscripten_query(sql.c_str());
		else {
			writeLog(LOG_LEVEL_ERROR, "Unsupported database type!");
			exit(3);
		}
	}

	int execute(string sql) {
		if (appConfig["database"].asString() == "mysql") return mysqli_execute(sql.c_str());
		else if (appConfig["database"].asString() == "sqlite") return sqlite_execute(sql.c_str());
		else if (appConfig["database"].asString() == "emscripten") return emscripten_execute(sql.c_str());
		else {
			writeLog(LOG_LEVEL_ERROR, "Unsupported database type!");
			exit(3);
		}
	}
};
#endif

#define itemNumberTemplate(item, filter) \
	string sql = "SELECT name FROM " defineToString(item) " WHERE (" + filter + ")"; \
    sql = "SELECT name FROM (" + sql + ") GROUP BY name"; \
    sql = "SELECT COUNT(*) AS sum FROM (" + sql + ")";
#define itemListTemplate(item, filter, order, st, en) if (order == "") order = "id DESC"; \
	string sql = "SELECT * FROM " defineToString(item) " WHERE (" + filter + ") ORDER BY " + order; \
    sql = "SELECT * FROM (" + sql + ") GROUP BY name"; \
    sql = "SELECT * FROM (" + sql + ") ORDER BY " + order + " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);      
                                                                                                                                                                        