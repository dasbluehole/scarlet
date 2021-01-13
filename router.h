#ifndef __ROUTER_H__
#define __ROUTER_H__
#include "request.h"

typedef void (*callback)(int c,req *r);
typedef struct r_entry
{
	char method[10];
	char uri[1024];    // requested path
	callback cb;   // destination
}r_entry;
typedef struct router
{
	r_entry *route;
	struct router *next;
	//int count;
}router;

router *create_router();
int add_route(router *r, char *method, char *uri, callback cb );
int do_router(router *rtr, int s, req* r);

#endif
