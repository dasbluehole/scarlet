#ifndef _REQPARSE_H_
#define _REQPARSE_H_
typedef struct request
{
    char method[10]; 
    char uri[4096]; //PATH_MAX
    char protocol[10];
}req;

req *parse_request(char *buf);

#endif