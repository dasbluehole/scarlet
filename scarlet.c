/////////////////////////////////////////////////
// scarlet.c                                   //
// scarlet http server                         //
// ashok shankar das                           //
/////////////////////////////////////////////////
/* 
 * this file implements only server part
 * the data handling part is in another file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memset() function */
#include <time.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <pthread.h>
#include "server_utils.h"
#include "mysock.h"
#include "util.h"


Server create_server(int port,char *ip)
{
	Server srv;
	//ok we will create a server socket
	int srv_sock=-1;
	srv_sock = create_server_socket(ip,port,SOCK_STREAM);
	if(srv_sock<0)
	{
		printf("Can not create a server socket\n");
		switch(srv_sock)
		{
			case -100 : printf("Unable to create socket\n");
				break;
			case -101 : printf("Unable to bind to port\n");
				break;
			case -102 : printf("unable to listen\n");
				break;
		}
		//exit(EXIT_FAILURE);
	}
	strcpy(srv.IP,ip);
	srv.port = port;
	strcpy(srv.name, SERVER);
	srv.socket = srv_sock;
	return(srv);
}

// this function will handle a request just after accept 
// till the process exits
void *send_recv_thread( void *arg ) {
	pthread_detach ( pthread_self() );
	Client *clnt =(Client *) malloc(sizeof(Client));
	memcpy(clnt, arg,sizeof(Client));
	free(arg);
	
	client_handle( clnt );
	close( clnt->socket );
	free(clnt);
	pthread_exit((void*) 0);
}
int main(int argc, char *argv[])
{
	
	//Creates an array of threads (Thread pool)
	pthread_t thread;
	
	if(argc != 3)
	{
		printf("Usage: %s <IP-address> <port_num>\n",argv[0]);
		exit(0);
	}
	char IP[15];
	strcpy(IP,argv[1]);
	if(validate_ip(IP)==1)
	{
		//printf("Valid IP address : %s\n",IP);
	}	
	else
	{
		printf("Invalid IP address : %s\n",IP);
		exit(0);
	}
	int srv_port = atoi(argv[2]);
	if(srv_port < 0)
	{
		printf("Invalid port\n");
		exit(EXIT_FAILURE);
	}
	//we created a server
	Server s = create_server(srv_port,IP);
	if(s.socket <0)
	{
		printf("Could not create a server hence exiting\n");
		exit(EXIT_FAILURE);
	}
	while(1)	// infinite server loop
	{
		struct sockaddr_in comming_addr;
		//socklen_t size = sizeof( struct sockaddr_in );
		int clnt_sock = accept_client(s.socket,(struct sockaddr*)&comming_addr);//,&size);
		if(clnt_sock<0)
		{
	      	/*We couldnot accept*/
			printf("accept error:...\n");
			perror("Accept:");
			//exit(EXIT_FAILURE);
			continue;
		}
		//good to go
		Client *c =(Client *)malloc(sizeof(Client));
		if(c==NULL)
		{
			printf("Memory allocation error for client\n");
			perror("malloc:");
			close(clnt_sock);
			close(s.socket);
			exit(EXIT_FAILURE);
		}
		c->socket = clnt_sock;
		c->port = htons(comming_addr.sin_port);
		strcpy(c->IP,inet_ntoa(comming_addr.sin_addr));
		//printf("Remote: %s on port %d \r\n", inet_ntoa(comming_addr.sin_addr), htons(comming_addr.sin_port));
		//here we should process the http request in a separate thread.
		if ( pthread_create( &thread, NULL, send_recv_thread, c) != 0 ) 
		{
		  perror( "pthread_create" );
		  close(c->socket);
		  close(s.socket);
		  exit(EXIT_FAILURE);
		}
	}
	close(s.socket); // we are done
	return(0);
}
