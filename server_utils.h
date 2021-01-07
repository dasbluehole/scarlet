/*
 * Network computing
 * 		Assignment 2: A HTTP program
 *      Written by Tran Quoc Hoan
 *			programmed by milestones method (step by step)
 *		
 *		ServerUtils.h		
 * 			Connect socket, socket handle
*/
#ifndef __SERVER_UTILS_H__
#define __SERVER_UTILS_H__
#include <stdio.h>
#define PORT "8080"  /* My port to listen on, in test server = uuid + 21600 */
#define LISTENQ 10  /* Default pass to listen */
#define MAXBUF 4096
#define EXIT_FAILURE 1
#define SERVER "Scarlet-0.1"
/* For debug */
//#define DEBUG
typedef struct Server
{
	int socket;
	int port;
	char name[31]; // name of server max 30 chars
	char IP[16]; // maximum 16 chars
}Server;

typedef struct Client
{
	int socket; // accept socket
	int port;	// port of client
	char IP[16]; // IP of client
	char session[128]; // session ID supplied by server after user authentication
}Client;

/* Error handling */
void err_handle( char * msg );

/* Send home page */
void send_home( int sock );

/* Send html header */
void send_header( int sock );

/* Send html data */
void send_data( int sock, FILE* fp );

/* Send html file */
void send_html( int sock, const char* FILENAME );

/* Handle for each socket */
int client_handle( Client *c );


#endif
