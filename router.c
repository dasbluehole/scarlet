#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "router.h"

extern void send_home(int sock);

router *create_router()
{
	router *r;
	r=(router*)malloc(sizeof(router));
	if(r==NULL)
	{
		printf("Unable to allocate memory for router\n");
		return NULL;
	}
	bzero(r,sizeof(router));
	r->route=NULL;
	r->next=NULL;
	//r->count = 0;
	return r;
}
int add_route(router *r, char *method, char *uri, callback cb )
{
	if(r==NULL)
		return -1;
	r_entry *re=(r_entry*)malloc(sizeof(r_entry));
	if(re==NULL)
	{
		printf("Error: allocationg memory\n");
		perror("Malloc:");
		return -1;
	}
	strcpy(re->method,method);
	strcpy(re->uri,uri);
	re->cb = cb;
	if(r->route == NULL)
	{
		r->route = re;
		//r->count++;
	}
	else
	{
		router *rtr = (router*)malloc(sizeof(router));
		rtr->route = re;
		rtr->next= NULL;
		while(r->next!=NULL)
			r=r->next;
		r->next = rtr;
	}
	return 1;
}
void dump_route(router *rtr)
{
	router *rt = rtr;
	while(rt)
	{
		printf("M: %s U%s\n",rt->route->method,rt->route->uri);
		rt=rt->next;
	}
}
int do_router(router *rtr, int s, req* r)
{
	router *rt=rtr;
	while(rt)
	{
		if(strcasecmp(rt->route->method,r->method)==0)
		{
			printf("Method = %s Uri = %s\n",r->method,r->uri);
			
			if(strcasecmp(rt->route->uri,r->uri)==0)
			{
				rt->route->cb(s,r);
				return(1);
			}
		}
		rt = rt->next;
	}
	//send_home(s);
	printf("noroute\n");
	//rt = rtr;
	//dump_route(rt);
	return(-1);
	
}
