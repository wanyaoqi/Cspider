#include "regex.h"

const char * HREF_PATTERN = "href=\"\\s*\\([^ >\"]*\\)\\s*\"";
regex_t reg;


void init_reg()
{
	if(regcomp(&reg,HREF_PATTERN,0)!=0)
	{
		printf("regcomp error\n");
		exit(-1);
	}
}

void free_reg()
{
	regfree(&reg);
}

void saveurl(char *buff,Curl &murl,int depth)
{	
	printf("save url start...\n");
	geturl(buff,murl,depth);
}

void geturl(char *&p,Curl &murl,int depth)
{
	int nmatch=2;
	regmatch_t matchptr[nmatch];
	char *tmp=NULL;
	int len=0;
	while (regexec(&reg, p, nmatch, matchptr, 0) != REG_NOMATCH) {
		printf("regexec is ok\n");
        len = (matchptr[1].rm_eo - matchptr[1].rm_so);
        p = p + matchptr[1].rm_so;
        tmp = (char *)calloc(len+3, 1);
        assert(tmp!=NULL);
        strncpy(tmp, p, len);
        tmp[len] = '\0';
        p = p + len + (matchptr[0].rm_eo - matchptr[1].rm_eo);
        murl.push_url(tmp,depth+1);
        free(tmp);
    }
}