#ifndef __CRAWLER_H__
#define __CRAWLER_H__
#include "sqlspider.h"
#include "url.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define MAX_DEPTH 200

char* itoa(int num,char*str,int radix);

bool get_ip_byname(char *domainname,char *hostip);
int creat_sock(char *domainname);
void makepost(char *domainname,char *url,char *postbuff);
void sendpost(char *domainname,char *url,int sockfd);
char *recvpost(int sockfd,int *msize);
bool savehtml(char *recvbuff,int size,char *path,MySqlServer &mySql);
char * get_domainname(char *url);

void get_and_prase_html(Curl &murl,Url &tmp,MySqlServer &mySql);















#endif