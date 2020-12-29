/*
 * Network computing
 * 		Assignment 2: A HTTP program
 *      Written by Tran Quoc Hoan
 *			programmed by milestones method (step by step)
 *		
 *		ServerUtils.h		
 * 			Connect socket, socket handle
*/

#define PORT "8080"  /* My port to listen on, in test server = uuid + 21600 */
#define LISTENQ 10  /* Default pass to listen */
#define MAXBUF 4096
#define EXIT_FAILURE 1

/* For debug */
//#define DEBUG

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
int sock_handle( int newsock );


