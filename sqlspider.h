#ifndef MySqlServer_H
#define MySqlServer_H

#include<iostream>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>
using namespace std;



class MySqlServer
{
public:
	MySqlServer();
	~MySqlServer();

	int insert_htmlfile(const char *url);
private:
	MYSQL *mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
};




#define DBPORT     3306
#define DBUSER     "root"

#define DBNAME     "spider"

#define TABLENAME  "urlfileid"
#define FILEID	   "fileid" 
#define URL  	   "furl" 



#endif