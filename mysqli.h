#ifndef MYSQLI_H
#define MYSQLI_H
using namespace std;

typedef vector<map<string,string> > mysqld;
MYSQL mysqli_connect(const char* host,const char* user,const char* passwd,const char* db,int port,string name="main-server") {
	MYSQL mysql,*res1; res1=mysql_init(&mysql); if (res1==NULL) cout << "Failed to initialize MYSQL structure!" << endl;
	bool res2=mysql_real_connect(&mysql,host,user,passwd,db,port,nullptr,0); if (!res2) cerr << mysql_error(&mysql) << endl;
	return mysql;
}

MYSQL mysql;

// Query Database
mysqld mysqli_query(MYSQL conn,const char* sql,string name="main-server") {
	mysqld res; map<string,string> tmp;
	bool res1=mysql_query(&conn,sql);
	if (res1) cerr << mysql_error(&conn) << endl;
	MYSQL_RES* res2=mysql_store_result(&conn);
	if (!res2) cerr << mysql_error(&conn) << endl;
	vector<string> field; MYSQL_FIELD* fd; MYSQL_ROW row;
	for (int i=0;fd=mysql_fetch_field(res2);i++) field.push_back(fd->name);
	while (row=mysql_fetch_row(res2)) {
		for (int i=0;i<field.size();i++) tmp[field[i]]=row[i];
		res.push_back(tmp);
	} mysql_free_result(res2);
	return res;
}

// Execute Database
void mysqli_execute(MYSQL conn,const char* sql,string name="main-server") {
	if (mysql_query(&conn,sql)) cerr << mysql_error(&conn) << endl;
}

#endif