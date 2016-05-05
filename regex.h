#ifndef __REGEX_H__
#define __REGEX_H__
#include "url.h"
#include <stdio.h>
#include <regex.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void init_reg();

void free_reg();

void saveurl(char *buff,Curl &murl,int depth);

void geturl(char *&p,Curl &murl,int depth);







#endif