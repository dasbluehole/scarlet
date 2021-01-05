#ifndef _REQPARSE_H_
#define _REQPARSE_H_
typedef struct request
{
    char method[10]; 
    char uri[4096]; //PATH_MAX
    char protocol[10];
}req;

req *parse_request(char *buf);

void do_get(int sock, req *r);
void do_post(int sock,req *r);
void do_unknown(int sock,req* r);
void do_put(int sock,req *r);
void do_del(int sock,req *r);
void do_head(int sock,req *r);

#endif
