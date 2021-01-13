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
#if 0
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
		printf("Found %x %x %x %x\n",_buf[strlen(_buf)-4],_buf[strlen(_buf)-3],_buf[strlen(_buf)-2],_buf[strlen(_buf)-1]);
		if(tail == NULL)
		{
			printf("Parse Error\n");
			printf("Found %x %x\n",_buf[strlen(_buf)-2],_buf[strlen(_buf)-1]);
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
#endif
#if 0
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
    char *_buf = strdup(buf);
    sscanf(_buf,"%s %s %s",r->method,r->uri,r->protocol);
    char *line = strtok(_buf,"\r\n");
    int head_cnt=0;
    line = strtok(NULL,"\r\n");
    int content_len =0;
    while(line)
    {
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
        }
        else
			break;
        if(head_cnt>=32 || p==NULL)
            break;
        line= strtok(NULL,"\r\n");
    }
    if(content_len)
	{
		r->payload = (char*)malloc(content_len);
		strcpy(r->payload,line);
	}
	int d = pos_empty_line(buf);
	//printf(">>>>>> %d \n",d);
	//printf("buf---->%s\n",buf);
//	printf("Remain->>> %s\n",line);
//	printf("Content len->>> %d\n",content_len);

    remove_spaces(r->method);
    remove_spaces(r->protocol);
    remove_duplicates(r->uri,'/');
	free(_buf);
    return(r);
}
#endif
#if 0
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
    char *_buf = strdup(buf);
    sscanf(_buf,"%s %s %s",r->method,r->uri,r->protocol);
    char *line = strtok(_buf,"\r\n");
    int head_cnt=0;
    line = strtok(NULL,"\r\n");
    int content_len =0;
    while(line)
    {
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
        }
        else
			break;
        if(head_cnt>=32 || p==NULL)
            break;
        line= strtok(NULL,"\r\n");
    }
    if(content_len)
	{
		r->payload = (char*)malloc(content_len);
		strcpy(r->payload,line);
	}
	int d = pos_empty_line(buf);
	printf(">>>>>> %d \n",d);
	printf("buf---->%s\n",buf);
//	printf("Remain->>> %s\n",line);
//	printf("Content len->>> %d\n",content_len);

    //remove_spaces(r->method);
    //remove_spaces(r->protocol);
    //remove_duplicates(r->uri,'/');
	free(_buf);
    return(r);
}
#endif
req *parse_request(char *buf)
{
	if(buf == NULL || strlen(buf)<=1)
		return(NULL);
	req *r = (req*)malloc(sizeof( req));
    if(r==NULL)
    {
		perror("malloc:");
		return(NULL);
    }
    bzero(r,sizeof(req));
    // allocated request header 
    char line[1024]; //allocate a character array to read lines from buffer
    char *_buf = buf;
    sscanf(_buf,"%[^\n]s",line); //read the first line 
							   // which contains method uri and protocol
	sscanf(line,"%s %s %s",r->method,r->uri,r->protocol);
	_buf = _buf+strlen(line)+1;  // advane buffer to next line
	int hdr_cnt = 0;
	int content_len =0;
	while(_buf[0]!='\0')
	{
		sscanf(_buf,"%[^\n]s",line);
#ifdef DEBUG
		printf("sz= %d ->%s\n",strlen(line),line);
#endif
		if(strlen(line) ==1 && content_len!=0)
		{
			// we are at the end of headers.
			// from here payload starts
			strcpy(r->payload,_buf+2);
			break;
		}
		_buf = _buf+strlen(line)+1;  // advane buffer to next line
		char *p=strchr(line,':');
		if (p!=NULL)
		{
			strncpy(r->harray[hdr_cnt].key,line,(int)(p-line));
			strcpy( r->harray[hdr_cnt].value,++p);
			if(strcasecmp(r->harray[hdr_cnt].key,"content-length")==0)
			{
				content_len = atoi(r->harray[hdr_cnt].value);
				//printf("content_len = %d\n",content_len);
				r->payload = malloc(content_len);
			}
			hdr_cnt++;
		}
		if(hdr_cnt>32)
		{
			// skip to empty line and copy the payload
			sscanf(_buf,"%[^\n]s",line);
			_buf = _buf + strlen(line)+1;
			if(strlen(line)==1)
				continue;
		}
	}
	return (r);
}
void dump_request(req *rq)
{
	if(rq !=NULL)
    {
        printf("Request--->\n");
        printf("%s %s %s\n", rq->method, rq->uri, rq->protocol);
        printf("Headers--->\n");
        for(int i = 0; i<32; i++)
        {
            if(strlen(rq->harray[i].key)<2)
                break;
            printf("%s : %s\n",rq->harray[i].key, rq->harray[i].value);
            
        }
        printf("payload = %s\n",rq->payload);
    }
    else
    {    
		printf("Error\n");
	}
}
