#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
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