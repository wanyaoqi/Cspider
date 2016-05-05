#include "sqlspider.h"

MySqlServer::MySqlServer()
{
	const char *host="127.0.0.1";
	const char *passwd="123456";
	mysql=new MYSQL;
	char buff[256]={0};
	if(NULL==mysql_init(mysql))
	{
		cout<<"Init fail. "<<mysql_error(mysql)<<endl;
		exit(-1);
	}
	if(!mysql_real_connect(mysql,host,DBUSER,passwd,NULL,DBPORT,NULL,0))
	{
		cout<<"Connect mysql fail. "<<mysql_error(mysql)<<endl;
		exit(-1);
	}
	if(mysql_select_db(mysql,DBNAME))
	{
		cout<<DBNAME<<" is not exist,creating database!"<<endl;
		memset(buff,0,sizeof(buff));
		snprintf(buff,sizeof(buff),"create database %s",DBNAME);
		if(mysql_real_query(mysql,buff,strlen(buff)))
		{
			cout<<"Create database "<<DBNAME<<" fail. "<<mysql_error(mysql)<<endl;
			exit(-1);
		}
		cout<<"Create database "<<DBNAME<<" success"<<endl;
		mysql_select_db(mysql,DBNAME);
	}
	res=mysql_store_result(mysql);
	if(res!=NULL)
		mysql_free_result(res);

	memset(buff,0,sizeof(buff));
	snprintf(buff,sizeof(buff),"select * from %s",TABLENAME);
	if(mysql_real_query(mysql,buff,strlen(buff)))
	{
		memset(buff,0,sizeof(buff));
		snprintf(buff,sizeof(buff),"create table %s(%s int(4) not null primary key auto_increment,%s char(128))",TABLENAME,FILEID,URL);
		res=mysql_store_result(mysql);
		if(res!=NULL)
			mysql_free_result(res);
		if(mysql_real_query(mysql,buff,strlen(buff)))
		{
			cout<<"Create table "<<TABLENAME<< "fail. "<<mysql_error(mysql)<<endl;
			exit(-1);
		}
		cout<<"Create table "<<TABLENAME<<" success"<<endl;
	}
	res=mysql_store_result(mysql);
	if(res!=NULL)
		mysql_free_result(res);
	res=NULL;
}


int MySqlServer::insert_htmlfile(const char *url)
{
	if(mysql==NULL)
	{
		cout<<"Connect database fail! "<<mysql_error(mysql)<<endl;
		return false;
	}
	char buff[256]={0};
	snprintf(buff,sizeof(buff),"insert into %s(%s) values('%s')",TABLENAME,URL,url);
	if(mysql_real_query(mysql,buff,strlen(buff)))
	{
		cout<<"Registration SQL failure! "<<mysql_error(mysql)<<endl;
		return false;
	}
	int fid=mysql_insert_id(mysql);
	res=mysql_store_result(mysql);
	if(res!=NULL)
		mysql_free_result(res);
	res=NULL;
	return fid;
}

MySqlServer::~MySqlServer()
{
	if(res!=NULL)
	{
		mysql_free_result(res);
		res=NULL;
	}
	mysql_close(mysql);
	delete mysql;
	mysql=NULL;
}