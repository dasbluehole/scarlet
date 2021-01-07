//request parser
#include "request.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "util.h"
int pos_empty_line(char *buf)
{
    int pos = -1;
    char *p = strstr(buf,"\r\n\r\n");
    if(p)
        pos= (p-buf);

    return (pos);
}
req* parse_request(char *buf)
{
	if(!buf || strlen(buf)<1)
		return(NULL);
    req *r = (req*)malloc(sizeof( req));
    if(r==NULL)
    {
		perror("malloc:");
		return(NULL);
    }
    bzero(r,sizeof(req));
    char *_buf = buf;
    int head_cnt=0;
    int content_len=0;
    int is_first_line=0;
	//get the first line
	char *tail=NULL, *line=NULL;
	int length;
	while(1)
	{
		
		
		tail = strstr(_buf,"\r\n");
		
		if(tail == NULL)
		{
			printf("Parse Error\n");
			free(r);
			return(NULL);
		}
		//printf("tail  at %p _buf at %p\n", tail,_buf);
		length = tail - _buf;
		if(length<1)
		{
			//printf("Empty line...length = %ld\n",length);
			break;
			//return NULL;
		}
		line = malloc(length+1);
		if(line ==NULL)
		{
			perror("Malloc:");
			free(r);
			return NULL;
		}
		strncpy(line, _buf, length);
		is_first_line++;
		tail +=2;
		_buf =tail; 
		// we will process the line here
		if(is_first_line==1)
		{
			sscanf(line,"%s %s %s",r->method,r->uri,r->protocol);
			free(line);
			continue;
		}
		char *p=strchr(line,':');
		if (p!=NULL)
		{
			strncpy(r->harray[head_cnt].key,line,(int)(p-line));
			strcpy( r->harray[head_cnt].value,++p);
			if(strcasecmp(r->harray[head_cnt].key,"content-length")==0)
			{
				content_len = atoi(r->harray[head_cnt].value);
			}
			head_cnt++;
			free(line);
		}
		else
		{
			free(line);
			break;
		}
		
		if(head_cnt>=32 || p==NULL)
			break;
	}
    // find the payload and copy it
    _buf=buf;
    char *pl = strstr(_buf,"\r\n\r\n");
    if(pl!=NULL)
    {
		if(content_len>0)
		{
			r->payload = malloc(content_len+1);
			bzero(r->payload,content_len+1);
			strncpy(r->payload,pl+4,content_len);
		}
	}
    return(r);
}
