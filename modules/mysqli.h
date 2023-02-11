#ifndef MYSQLI_H
#define MYSQLI_H
using namespace std;

typedef vector<map<string,string> > mysqld;
MYSQL mysqli_connect(const char* host,const char* user,const char* passwd,const char* db,int port,string name="main-server") {
	MYSQL mysql,*res1; res1=mysql_init(&mysql); 
	if (res1==NULL) cout << "Failed to initialize MYSQL structure!" << endl;
	bool res2=mysql_real_connect(&mysql,host,user,passwd,db,port,nullptr,CLIENT_MULTI_STATEMENTS); 
	if (!res2) cerr << mysql_error(&mysql) << endl;
	return mysql;
}

MYSQL mysql;

// Query Database
//! The parameter `conn` is decrypted. We will remove it in the future.
mysqld mysqli_query(MYSQL conn,const char* sql,string name="main-server") {
	MYSQL mysql = mysqli_connect(appConfig["mysql.hostname"].asString().c_str(), appConfig["mysql.username"].asString().c_str(), 
        appConfig["mysql.password"].asString().c_str(), appConfig["mysql.database"].asString().c_str(), appConfig["mysql.port"].asInt());
	bool res1 = mysql_query(&mysql,sql);
	if (res1) cerr << mysql_error(&mysql) << endl;
	MYSQL_RES* res2=mysql_store_result(&mysql);
	if (!res2) cerr << mysql_error(&mysql) << endl;
	vector<string> field; MYSQL_FIELD* fd; MYSQL_ROW row;
	mysqld res; map<string,string> tmp;
	for (int i=0;fd=mysql_fetch_field(res2);i++) field.push_back(fd->name);
	while (row=mysql_fetch_row(res2)) {
		for (int i=0;i<field.size();i++) tmp[field[i]] = row[i] == NULL ? "" : row[i];
		res.push_back(tmp);
	} tmp.clear();
	delete[] row;
	mysql_free_result(res2);
	mysql_close(&mysql);
	return res;
}

// Execute Database
//! The parameter `conn` is decrypted. We will remove it in the future.
int mysqli_execute(MYSQL conn,const char* sql,string name="main-server") {
	MYSQL mysql = mysqli_connect(appConfig["mysql.hostname"].asString().c_str(), appConfig["mysql.username"].asString().c_str(), 
        appConfig["mysql.password"].asString().c_str(), appConfig["mysql.database"].asString().c_str(), appConfig["mysql.port"].asInt());
	if (mysql_query(&mysql,sql)) cerr << mysql_error(&mysql) << endl;
	int rows = mysql_affected_rows(&mysql);
	mysql_close(&mysql);
	return rows;
}

#endif