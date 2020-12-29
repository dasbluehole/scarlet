//request parser
#include "request.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    memset( &method, 0, sizeof( method ) );
    memset( &url, 0, sizeof( url ) );
    memset( &http_ver, 0, sizeof( http_ver ) );
    sscanf(buf,"%s %s %s",r->method,r->uri,r->protocol);
    
    remove_spaces(r->method);
    remove_spaces(r->protocol);
    
    return(r);
}
