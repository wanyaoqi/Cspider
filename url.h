#ifndef __URL_H__
#define __URL_H__
#include <stdio.h>
#include <set>
#include <queue>
#include <string>
#include <iostream>
#include <string.h>
using namespace std;


struct Url 
{
	string url;
	int depth;
public:
	Url(string murl,int mdepth):url(murl),depth(mdepth){}
};
class Curl
{
 public:
 	set<string>   s_url;
 	queue<Url >    q_url;
 public:
 	void push_url(char *url,int depth)
 	{
 		if(strncmp(url,"http://",7)==0)
 		{
 			string m_url(url);
 			if(s_url.find(m_url)==s_url.end())
 			{
 				printf("url=%s\n",url);
 				s_url.insert(m_url);
 				q_url.push(Url(m_url,depth));
 				printf("q_url.size=%d\n",q_url.size());
 			}
 		} 		
 	}
};


 #endif