#include "crawler.h"
#include "url.h"
#include "sqlspider.h"
#include "regex.h"
#define SIZE (1024*1024)



void startwork(int sockfd,char *arg,MySqlServer &mySql)
{
	printf("startwork\n");
	int recvsize=0;
	char *recvbuff=recvpost(sockfd,&recvsize);
	printf("recvsize=%d\n",recvsize);
	Curl murl;
	int depth=0;

	char *p=recvbuff;
	saveurl(p,murl,depth);

	savehtml(recvbuff,recvsize,arg,mySql);
	while(murl.q_url.size()>0)
	{
		Url tmp=murl.q_url.front();
		murl.q_url.pop();
		get_and_prase_html(murl,tmp,mySql);
	}
}

int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("arguments of main are not enough\n");
		exit(-1);
	}
	char *domainname=get_domainname(argv[1]);//分离域名
	//printf("%s\n",domainname);
	int sockfd = creat_sock(domainname);//创建套接字
	if(sockfd==-1)
	{
		printf("creat socket failed\n");
	}
	sendpost(domainname,argv[1],sockfd);//封装http头并发送
	MySqlServer mySql;
	init_reg();
	startwork(sockfd,argv[1],mySql);//work begin	
	//close(sockfd);
}



