//////////////////////////////////////////////////////////////
//Al Sherawi Rack Light stage relay control server          //
// Copyright (C) J Systems and Controls (Jsyscon) 2012,2013 //
// Developed by Ashok Shankar Das (ashok.das@jsyscon.in)    //
//////////////////////////////////////////////////////////////
// mysock.h is a wrapper over UNIX BSD socket               //
//////////////////////////////////////////////////////////////
#ifndef MYSOCK_H
#define MYSOCK_H
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libconfig.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <features.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <sys/ioctl.h>
#include <net/if.h>

int max_con = 10;

/*  int create_server_socket(int)
    takes the port number an unsigned integer
    sock_type the socket type SOCK_STREAM,SOCK_DGRAM
    returns socket handle on success or the error response from APIs
    This function creates a server socket.   
    this function is needed to create proxy server socket 
*/
//RETURN CODES on success socket returns a positive value. hence we return that
// else -100 for socket creation error
//      -101 for bind error
//      -102 for listen error

int create_server_socket(char *IPstring, unsigned int serv_port,int sock_type)
{
	int sockfd;			/*listen on sockfd */
	struct sockaddr_in my_addr;	/*struct of my socket and there socket*/
	//int sin_size;
	int res,i=1;			/*var to store the returned value of func.s
						  to chek for success or failure*/
	//if(max_con<30)
	//    max_con=30;			// limit the number of connections to 30 if it is less than 30
	/*create a socket handle*/
	sockfd=socket(AF_INET,sock_type,0);
	if(!sockfd)
	{
		/*we are unable to create a socket*/
		printf("Unable to create a socket\n Quiting....\n");
		return -100;
	}
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
	/*fill up the my_addr struct*/
	memset(&my_addr,'\0',sizeof(struct sockaddr_in));	/*cleanup the mem*/
	my_addr.sin_family=AF_INET;			/*socket family*/
	my_addr.sin_port=htons(serv_port);			/*socket port*/
	my_addr.sin_addr.s_addr=inet_addr(IPstring);		/*Put My IP from the system*/

	/*Bind the socket to the structure*/
	res=bind(sockfd,(struct sockaddr*) &my_addr, sizeof(struct sockaddr));
	if(res<0)
	{
		/*we could not bind*/
		printf("Bind error: \n");
		perror("");
		return -101;
	}
	/*Listen if any body is calling us*/
	res=listen(sockfd,max_con);				/*we only handle 10 calls*/
	if(res<0)
	{
		/*we can't listen*/
		printf("Error listening: \n");
		return -102;
		//close(sockfd);
	}
	return sockfd; /* successfully we created a server socket*/
}
// This function creates a client soket and returns its descriptor
// this function takes host_ip, portnumber and socket type for creating the socket
// this returns error code -100 if socket could not be created
// -103 if can not connect to the server at port number

int create_client_socket(char *host_ip,int port_to_connect, int sock_type)
{
    //IMPLEMENT THE CLIENT STUFF HERE
    struct sockaddr_in client_sock;
    int client_desc,i=1;
    bzero((char *)&client_sock,sizeof(client_sock)); //zero out the client_sock 
    client_sock.sin_family=AF_INET;
    client_sock.sin_port=htons(port_to_connect);
    client_sock.sin_addr.s_addr=inet_addr(host_ip);
    client_desc=socket(AF_INET,sock_type,0);
    if(client_desc<0)
    {
	printf("Unable to create socket\n");
	return -100;
    }
    setsockopt(client_desc,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
    if(connect(client_desc,(struct sockaddr*)&client_sock,sizeof(client_sock))<0)
    {
	printf("Error Connecting: \n");
	return -103;
    }
    return client_desc;
}    
//accept_client(...) function takes 2 params.
//	server_sock an integer type generally returned by create_server_socket(...)
//	struct_client_addr a pointer to the client address structure of type sockaddr_in
//THE RETURN VALUE returns the return value of accept
int accept_client(int  server_sock, struct sockaddr *struct_client_addr)
{
    int temp;
    socklen_t siz;
    siz=(socklen_t)sizeof(struct sockaddr_in);
    temp=accept(server_sock, struct_client_addr, &siz);
    return temp;
}
int accept4_client(int  server_sock, struct sockaddr *struct_client_addr, socklen_t *siz)
{
    int temp;
    //socklen_t siz;
    //siz=(socklen_t)sizeof(struct sockaddr_in);
    temp=accept4(server_sock, struct_client_addr, siz,SOCK_NONBLOCK);
    return temp;
}
// The following function returns an associated file handle(FILE*) for socket descriptor sock. 
// the mode is mode for opening the file like r,w,r+,w+ etc...
FILE * socket_to_file(int sock,const char *mode)
{
    FILE * fp;
    fp=fdopen(sock,mode);
    return fp;
}
// this is to check if the socket is ready for read
int is_socket_ready_for_read(int sock_fd)
{
    fd_set fdset;
    struct timeval timeout;
    timeout.tv_sec=10; //time out in 10secs coz we r checking
    timeout.tv_usec=0;
    FD_ZERO(&fdset);
    FD_SET(sock_fd,&fdset);
    select(sock_fd+1,&fdset,NULL,NULL,&timeout);
    if(FD_ISSET(sock_fd,&fdset))
	return 1;
    return 0;
}
// this is to check if the socket is ready for write
int is_socket_ready_for_write(int sock_fd)
{
    fd_set fdset;
    struct timeval timeout;
    timeout.tv_sec=10; //time out in 10secs coz we r checking
    timeout.tv_usec=0;
    FD_ZERO(&fdset);
    FD_SET(sock_fd,&fdset);
    select(sock_fd+1,NULL,&fdset,NULL,&timeout);
    if(FD_ISSET(sock_fd,&fdset))
	return 1;
    return 0;
}

// IN PLACE OF get_line_from WE ARE USING THIS FUNCTION
// returns a character buffer allocated 
// Input socket and address of status, address of bytes_read
char *getSocketLine(int sock,int *status, int *byt_rd)
{
	int maxlen=200;
	int i;
	////alloclen=(maxlen>100?100:maxlen);
	int selstatus;
	fd_set fdset;
	struct timeval timeout;
	static char *buf=NULL;
	static int alloclen=1000; //upto max 1000 char in a line we accept
	i=0;
	*status=0;
	if (buf == NULL)
	{
	    buf=(char *)malloc(alloclen*sizeof(char));
	    if(!buf)
	    {
		*status=3;
	        return NULL;
	    }
	}
	bzero(buf,alloclen);
	while(i<maxlen&&(i==0||buf[i-1]!='\n')){
		if(i==alloclen-1){
		       	buf=(char *)realloc(buf,alloclen+100);
			alloclen+=100;
		}
		FD_ZERO(&fdset);
		FD_SET(sock,&fdset);
		timeout.tv_sec=30;
		timeout.tv_usec=0;
		if((selstatus=select(sock+1,&fdset,NULL,NULL,&timeout))==0){
			printf("time out\n");
			fflush(stdout);
			*status=2;
			break;
		}
		if(read(sock,&buf[i],1)==0) {//connection closed
			*status=1;
			break;
		}
		i++;
	}
	buf[i]='\0';
	return buf;
}
int sendSocketLine(int sock,char *buf){
	int i;
	int selstatus;
	fd_set fdset;
	struct timeval timeout;
	i=-1;
	do {
		i++;
		FD_ZERO(&fdset);
		FD_SET(sock,&fdset);
		timeout.tv_sec=30;
		timeout.tv_usec=0;
		if((selstatus=select(sock+1,NULL,&fdset,NULL,&timeout))==0){//timedout
			printf("time out\n");
			fflush(stdout);
			return 0;
		}
		if(write(sock,&buf[i],1)==0){//connection closed
			return 0;
		}
	}while(buf[i]!='\n');
	return i+1;
}
#endif //MYSOCK_H
