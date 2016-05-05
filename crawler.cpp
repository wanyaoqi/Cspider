#include "crawler.h"
#include "regex.h"
#include <sys/time.h>



static void * mymalloc()
{

}

char* itoa(int num,char*str,int radix)
{/*索引表*/
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;/*中间变量*/
	int i=0,j,k;
	/*确定unum的值*/
	if(radix==10&&num<0)/*十进制负数*/
	{
		unum=(unsigned)-num;
		str[i++]='-';
	}
	else unum=(unsigned)num;/*其他情况*/
	/*转换*/
	do{
		str[i++]=index[unum%(unsigned)radix];
		unum/=radix;
	}while(unum);
	str[i]='\0';
/*逆序*/
	if(str[0]=='-')k=1;/*十进制负数*/
	else k=0;
	char temp;
	for(j=k;j<=(i-1)/2;j++)
	{
		temp=str[j];
		str[j]=str[i-1+k-j];
		str[i-1+k-j]=temp;
	}
	return str;
}






void get_and_prase_html(Curl &murl,Url &tmp,MySqlServer &mySql)
{
	if(tmp.depth>=MAX_DEPTH)
	{
		return;
	}
	char *url=(char *)malloc(1024);
	assert(url!=NULL);
	strcpy(url,tmp.url.c_str());

	char *domainname=get_domainname(url);

	int sockfd=creat_sock(domainname);
	if(sockfd==-1)
	{
		free(domainname);
		free(url);
		return ;
	}
	sendpost(domainname,url,sockfd);

	int recvsize=0;
	char *recvbuff=recvpost(sockfd,&recvsize);
	printf("recvbuff size=%d\n",recvsize);
	if(recvsize==0)
	{
		free(recvbuff);
		free(url);
		return ;
	}

	saveurl(recvbuff,murl,tmp.depth);

	savehtml(recvbuff,recvsize,url,mySql);
	free(url);
}










bool get_ip_byname(char *domainname,char *hostip)
{
	if(domainname == NULL)
	{
		printf("domainname is NULL\n");
		return false;
	}

	struct hostent *host;

	if((host = gethostbyname(domainname)) == NULL)
	{		
		printf("can not get it\n");
		return false;
	}
	else
	{
		strcpy(hostip,inet_ntoa(*((struct in_addr *)host->h_addr)));
		return true;
	}	
}

int creat_sock(char *domainname)
{	
	int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd==-1)
	{
		perror("socket error");
		return -1;
	}
	
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(80);
	
	char hostip[32] = "";
	if(get_ip_byname(domainname,hostip))
	{
		//printf("host:%s\n",domainname);
		//printf("ip  :%s\n\n",hostip);
		saddr.sin_addr.s_addr = inet_addr(hostip);
	}
	else
	{
		printf("get ip error \n");
		return -1;
	}
	struct timeval ntime={5,0};
    setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(char *)&ntime,sizeof(struct timeval));
	int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	if(res == -1)
	{
		perror("connect error");
		close(sockfd);
		return -1;
	}
	
	return sockfd;
}
void makepost(char *domainname,char *url,char *postbuff)
{
    sprintf(postbuff,"GET %s HTTP/1.1\r\nHost: ",url); 
    strcat(postbuff,domainname);  
    strcat(postbuff, "\r\nConnection:Close\r\n\r\n"); 
}

void sendpost(char *domainname,char *url,int sockfd)
{
	int size = sizeof(char)*1024;
	char *postbuff = (char *)malloc(size);
	assert(postbuff != NULL);

	makepost(domainname,url,postbuff);

	if(send(sockfd,postbuff,strlen(postbuff),0) == -1)
	{
		printf("send error!\n\n");
	}
	printf("post sended :\n%s\n",postbuff);
	free(postbuff);
	free(domainname);
}

char *recvpost(int sockfd,int *msize)
{
	int size = sizeof(char)*1024*1024*2;
	char *recvbuff = (char *)malloc(size);
	assert(recvbuff!=NULL);
	char *tmp=recvbuff;
	assert(recvbuff != NULL);

	memset(recvbuff,0,size);
	int ret=0;

	while((ret=recv(sockfd,recvbuff,size,0)) > 0)
	{		
		recvbuff=recvbuff+ret;
		*msize+=ret;
	}
	
	printf("recv seccess,saving html...\n");
	close(sockfd);
	return tmp;	
}

int openfile(int ret)
{
	char tmp[12]={0};
	itoa(ret,tmp,10);
	char path[128]="/home/wyq/cspider/html/";
	strcat(path,tmp);
	int fd=open(path,O_CREAT | O_WRONLY,0600);
	assert(fd!=-1);
	return fd;
}

bool savehtml(char *recvbuff,int size,char *path,MySqlServer &mySql)
{
	int ret=mySql.insert_htmlfile(path);
	int fw=openfile(ret);
	char *tmp=recvbuff;
	int i=0;
	while(strncmp(recvbuff,"\r\n\r\n",4)!=0)
	{
		recvbuff++;
		i++;
	}
	recvbuff=recvbuff+4;
	i+=4;
	if(write(fw,recvbuff,size-i) == -1)
	{
		printf("saving html error!\n\n");
		return false;
	}	
	close(fw);
	free(tmp);
	return true;
}

char * get_domainname(char *url)
{
	char *domainname=(char *)malloc(128);
	assert(domainname!=NULL);
	int i= 0;
	if(strncmp(url,"http://",7)==0)
	{
		url=url+7;
	}	
	while((*url!='/')&&(*url!='\0'))
	{
		domainname[i++]=*url++;
	}
	domainname[i]='\0';
	return domainname;
}



/*
int main(int argc,char **argv)
{

	if(argc < 2)
	{
		printf("arguments of main are not enough\n");
		exit(-1);
	}
	char *domainname=get_domainname(argv[1]);//fenliyuming
	printf("%s\n",domainname);
	int sockfd = creat_sock(domainname);//chuangjiantaojiezi

	return 0;
}
*/