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
#include "mysock.h"
// remove spaces from the string
//https://stackoverflow.com/users/2682012/ericcurtin
void remove_spaces(char* s) 
{
   const char* d = s;
   do
   {
   	while (*d == ' ')
   	{
   	    ++d;
   	}
   } while (*s++ = *d++);
}
int count_dots(char *ip)
{
   int cnt=0;
   while(*ip)
   {
   	if(*ip=='.')
   	    cnt++;
   	ip++;
   }
   return(cnt);
}
int validate_ip(char *ip)
{
    // strip spaces if any
    remove_spaces(ip);
    //IPv4 IP address has ppp.qqq.rrr.sss
    //so we will see if there are 4 substrings and 3 delemeters
    if(count_dots(ip)!=3)
    {
    	printf("Invalid format IP\n");
    	return(-1);
    }
    char *str, *substr;
    str = strdup(ip);
    while( (substr = strsep(&str,".")) != NULL )
    {
        //printf("%s\n",substr);
        if((atoi(substr)>255) || (atoi(substr)<0))
        {
           printf("Bad IP\n");
           return(-2);
        }
    }
    return(1);   
}
// this function will handle a request just after accept 
// till the process exits
void *send_recv_thread( void *arg ) {
	pthread_detach ( pthread_self() );
	int acc = (int) arg;
	
	sock_handle( acc );
	close( acc );
	pthread_exit((void*) 0);
}
int main(int argc, char *argv[])
{
	
	//Creates an array of threads (Thread pool)
	pthread_t *thread_id = (pthread_t *)malloc(sizeof(pthread_t)*500);
	bzero(thread_id,sizeof(pthread_t)*500);
	//Used to keep track of which threads have been used.
	int inUse[500];// = {0,0,0,0,0,0,0,0,0,0};
	bzero(inUse,500*sizeof(int));
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
	//ok we will create a server socket
	int srv_sock=-1, clnt_sock=-1;
	srv_sock = create_server_socket(IP,srv_port,SOCK_STREAM);
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
		exit(EXIT_FAILURE);
	}
	
	//we created a socket
	//while(1)	// infinite server loop
	int i = 0;
	while( i > -1 && i < 500 )
	{
		struct sockaddr_in comming_addr;
		socklen_t size = sizeof( struct sockaddr_in );
		clnt_sock = accept_client(srv_sock,(struct sockaddr*)&comming_addr);//,&size);
		if(clnt_sock<0)
		{
	      	/*We couldnot accept*/
			printf("accept error:...\n");
			perror("Accept:");
			//exit(EXIT_FAILURE);
			continue;
		}
		//good to go
		printf("Remote: %s on port %d \r\n", inet_ntoa(comming_addr.sin_addr), htons(comming_addr.sin_port));
		//here we should process the http request in a separate thread.
		if ( pthread_create( &thread_id[i], NULL, send_recv_thread, (void *)clnt_sock ) != 0 ) 
		{
		  perror( "pthread_create" );
		}
		inUse[i] = 1;
		i++;
		int iIsSet = 0;
		int j;
		//TODO: CLEAN UP THE THREAD POOL
		for( j = 0 ; j < 500 ; j++ ) 
		{
			if (inUse[j] == 1)
			{
				printf("Checking the status of thread (%d)\n",j);
				if(pthread_kill(thread_id[j], 0) == 0 )
				{
					printf("Thread (%d) is alive\n", j);
					//TODO: Count number of alive threads.  
					//determine global stay alive time
				}	
				else
				{
				//Kills all inactive threads.
					int k = pthread_kill(thread_id[j], 1);
					printf("Thread (%d) has been terminated\n",j);
					if (iIsSet == 0)
					{
						//Set i to the lowest thread in the pool
						i = j;
						iIsSet = 1;
					}
				}
			}
			if (j == 499 && i > 499)
			{
				//If here, thread pool is full.  Wait for 10 seconds, 
				//then clean the pool again
				sleep(10);
				j = 0;
			}
		}
	}
	close(srv_sock); // we are done
	return(0);
}
