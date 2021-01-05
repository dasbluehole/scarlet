//request parser
#include "request.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "util.h"

req* parse_request(char *buf)
{
    req *r = (req*)malloc(sizeof(req));
    if(r==NULL)
    {
		perror("malloc:");
		return(NULL);
    }
    bzero(r,sizeof(req));
    char method[256], url[256], http_ver[256];
    bzero( &method, sizeof( method));
    bzero( &url, sizeof(url));
    bzero( &http_ver, sizeof(http_ver));
    sscanf(buf,"%s %s %s",r->method,r->uri,r->protocol);
    
    remove_spaces(r->method);
    remove_spaces(r->protocol);
    remove_duplicates(r->uri,'/');
    return(r);
}
