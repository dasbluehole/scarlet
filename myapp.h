//myapp.h
#ifndef __MYAPP_H__
#define __MYAPP_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include "server_utils.h"
#include "request.h"
#include "router.h"
void send_index(int s, req *r)
{
	printf("send index\n");
	send_html(s,"/var/www/index.html");
}
void do_test(int s, req* r)
{
    printf("Testing test.html\n");
    send_html(s,"/var/www/test.html");
}
void send_login(int c, req *r)
{
	printf("send login\n");
	send_html(c,"/var/www/login.html");
}
void validate(int c, req *r)
{
	char uid[33],pwd[16];
	char *_buf=NULL;
	char outbuf[ MAXBUF + 1];
	if(r->payload !=NULL)
	{
		char *buf = (char*)malloc(strlen(r->payload)+1);
		_buf= buf;
		strcpy(buf,r->payload);
		char *p=strcasestr(buf,"username=");
		if(p == NULL)
			goto end;
		buf=buf+9;
		p= strchr(buf,'&');
		if(p == NULL)
			goto end;;
		strncpy(uid,buf,p-buf);
		if(strcasecmp(uid,"ashok")==0)
			goto ok;
	}
end:	// do error here
	send_html(c,"/var/www/404.html");
	free(_buf);
	return;
ok:
	
	memset( &outbuf, 0, sizeof(outbuf) );
	
	// get time now
	time_t timer = time( NULL );
	struct tm *date = localtime( &timer );
    
	snprintf( outbuf, sizeof(outbuf), 
  	    "HTTP/1.0 200 OK\r\n"
  	    "Server: %s\r\n"
  	    "Content-Type:text/html\r\n"
		"Date: %s\r\n"
		"Connection: Closed\r\n"
		"<font color=red><h1>Success</h1></font>\r\n", SERVER,
		asctime( date ) );
    send( c, outbuf, (int)strlen(outbuf), 0);
	free(_buf);
}


#endif
