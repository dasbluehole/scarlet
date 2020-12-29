/*
 * Network computing
 * 		Assignment 2: A HTTP program
 *      Written by Tran Quoc Hoan
 *			programmed by milestones method (step by step)
 *		
 *		ServerUtils.c		
 * 			Connect socket, socket handle
*/
#include <stdio.h>
#include <string.h>		/* for memset() function */
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>

#include "server_utils.h"
#include "request.h"

#define TRUE 1
#define FALSE 0
#define WORK_DIR "/var/www"
/* Error handling */
void err_handle( char * msg ) {
	perror( msg );
	exit ( EXIT_FAILURE );
}

/* Send home page */
void send_home( int sock ) {
	char outbuf[ MAXBUF + 1];
	memset( &outbuf, 0, sizeof(outbuf) );
	
	// get time now
	time_t timer = time( NULL );
	struct tm *date = localtime( &timer );
    
	snprintf( outbuf, sizeof(outbuf), 
  	    "HTTP/1.0 200 OK\r\n"
  	    "Content-Type:text/html\r\n"
		"Date: %s\r\n"
		"<font color=red><h1>404 Not Available.</h1></font>\r\n", 
		asctime( date ) );
    send( sock, outbuf, (int)strlen(outbuf), 0);
#ifdef DEBUG
	printf("Finish send_home \n");
#endif
	return;
}

/* Send html header */
void send_header( int sock ) {
	char outbuf[ MAXBUF + 1];
	memset( &outbuf, 0, sizeof(outbuf) );
	
	// get time now
	time_t timer = time( NULL );
	struct tm *date = localtime( &timer );
    
	snprintf( outbuf, sizeof(outbuf), 
  	    "HTTP/1.0 200 OK\r\n"
  	    "Content-Type:text/html\r\n"
		"Date: %s\r\n", asctime( date ) );
    send( sock, outbuf, (int)strlen(outbuf), 0);
#ifdef DEBUG
	printf("Finish send_header \n");
#endif
	return;
}

/* Send html data */
void send_data( int sock, FILE* fp ) {
	int len;
	char outbuf[ MAXBUF + 1];
	memset( &outbuf, 0, sizeof(outbuf) );
	
	while ( !feof(fp) ) {
		len = fread( outbuf, 1, sizeof( outbuf ), fp );
		outbuf[len] ='\0';
		send ( sock, outbuf, len, 0 );
	}
#ifdef DEBUG
	printf("Finish send_data \n");
#endif
	return;
}

/* Send html file */
void send_html( int sock, const char* FILENAME ) {
	FILE* fp;
	fp = fopen( FILENAME, "r" ); 
	if ( fp == NULL ) {
		// send html header only
		send_home( sock );
	} else {
		send_header( sock );
		send_data( sock, fp );
		fclose( fp );
	}
#ifdef DEBUG
	printf("Finish send_html \n");
#endif
	return;
}

/* Handle for each socket */
int sock_handle( int newsock ) {
  	/* recv(), send(), close() */

  	// recv()
	char inbuf[ MAXBUF + 1];
	//char *inbuf;
    memset( &inbuf, 0, sizeof(inbuf) );
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    if (setsockopt (newsock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        printf("setsockopt failed\n");

    if (setsockopt (newsock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        printf("setsockopt failed\n");
    if ( recv( newsock, inbuf, sizeof(inbuf), 0 ) < 0 ) {
    	close( newsock );
		perror("recv");
		return FALSE;
    };
    /*int status=read(socket, &inbuf, 255); 
    if(status<0) 
    {
		printf("There was an error reading the input");
		return(FALSE);
	}*/
     

#ifdef DEBUG
    printf( "%s", inbuf );
#endif
	
	// send()
/*	char method[256], url[256], http_ver[256];
	char *request_file;
	
	memset( &method, 0, sizeof( method ) );
	memset( &url, 0, sizeof( url ) );
	memset( &http_ver, 0, sizeof( http_ver ) );
	
	sscanf( inbuf, "%s %s %s", method, url, http_ver );
*/
	req *r = parse_request(inbuf);
	if(r==NULL)
	{
	    printf("Unable to parse request\n");
	    return(FALSE);
	}
	//printf("method %s uri %s proto %s \n",r->method,r->uri,r->protocol);
	if(strcasecmp(r->method,"GET")==0)
	    do_get(newsock,r);
	else if(strcasecmp(r->method,"POST")==0)
	    do_post(newsock,r);
	else if(strcasecmp(r->method,"PUT")==0)
	    do_put(newsock,r);
	else if(strcasecmp(r->method,"DELETE")==0)
	    do_del(newsock,r);
	else if(strcasecmp(r->method,"HEAD")==0)
	    do_head(newsock,r);
	else
	    do_unknown(newsock,r);
	
	// get method
/*	if ( !strncasecmp( method, "GET", strlen( "GET" ) ) ) {
		// request page is slash "/"
		
			// sending index file
			request_file = "index.html";
		}
		else {
			// exclude "/"
			request_file = url + 1;
		}
		send_html( newsock, request_file );
	}
*/	
    if(r)
    {
	free(r);
	r = NULL;
    }
  return TRUE;
}

void do_get(int sock,req *r)
{
    char request_file[1024]="";
    strcpy(request_file,WORK_DIR);
    // for the time being we will serv only disk files no virtual resource
    if(!strncmp(r->uri,"/",strlen("/")) && (strlen(r->uri) == 1 ) ) 
    //if(strncmp(r->uri,"/",strlen("/")==0) && strlen(r->uri)==1)
    {
	// landing page or resource
	// for the time being index.html
	strcat(request_file ,"/index.html");
    }
    else
    {
//	strcpy(request_file,WORK_DIR);
	strcat(request_file,r->uri);
    }
    //printf("request file = %s\n",request_file);
    send_html(sock,request_file);
}
void do_unknown(int sock, req* r)
{
    char outbuf[ MAXBUF + 1];
    memset( &outbuf, 0, sizeof(outbuf) );
    // get time now
    time_t timer = time( NULL );
    struct tm *date = localtime( &timer );
    snprintf( outbuf, sizeof(outbuf), \
    "HTTP/1.0 200 OK\r\n" \
  	    "Content-Type:text/html\r\n" \
		"Date: %s\r\n" \
		"<font color=red><h1>HELLO! Hacker! The feature is NOT Implemented yet.</h1></font>\r\n", \
		asctime( date ) );
    send( sock, outbuf, (int)strlen(outbuf), 0);
}
void do_post(int sock,req *r)
{
    do_unknown(sock,r);
}

void do_put(int sock,req *r)
{
    do_unknown(sock,r);
}

void do_del(int sock,req *r)
{
    do_unknown(sock,r);
}
void do_head(int sock,req *r)
{
    do_unknown(sock,r);
}
