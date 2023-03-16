/*********************************************************************************
 *
 *
 *      Copyright:  (C) 2022 lan li
 *                  All rights reserved.
 *
 *       Filename:  socket_client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/20/22)
 *         Author:  Lan li
 *      ChangeLog:  1, Release initial version on "10/20/22 12:56:31"
 *                 
 ********************************************************************************/
#include <sys/types.h>         
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <netdb.h>

#define MSG_STR "Hello world"


void print_usage(char *program)
{
	printf("%s usage :\n",program);
	printf("-i(--ipaddr): sepcify server IP address\n");
	printf("-p(--port): sepcify server \n");
	printf("-d(--domain):sepcify server domain\n");
	printf("-h(--help): print this help information\n");
 
}


int main(int argc, char *argv[])
{

	int 				sockfd=-1;
	int 				rv=-1;
	struct sockaddr_in 	servaddr;
	char				*servip=NULL;
	int					port;
	char				buf[1024];
	int 				ch;
	struct	hostent		*host;
	char				str[32];
	char				*domain=NULL;
	char				**pIP=NULL;
	struct option		opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument ,NULL,'h'},
		{"doamin",required_argument,NULL,'d'},
		{NULL,0,NULL,0}
	};
	printf("start parser arguments\n");


	while ((ch=getopt_long(argc,argv,"i:p:d:h",opts,NULL))!=-1)
	{
		printf("ch:%c\n",ch);
		switch(ch)
		{
			case'i':
				servip=optarg;

				break;
			case 'p':
				port=atoi(optarg);
				break;
			case 'd':
				domain =optarg;
				break;
			case 'h':
				print_usage(argv[0]);
			default:
				printf("Unknown return val:%d\n",ch);
		}
	}
	if(!(!servip^!domain) | !port)
	{
		print_usage(argv[0]);
		return -1;
	}

	if(!servip)
	{
		if((host = gethostbyname(domain)) == NULL)
		{
			printf("gethostbyname error: %s\n", strerror(errno));
			return -1;
		}
		printf("gethostbynameyname successfully!\n");
		switch(host->h_addrtype)
		{
			case AF_INET:

			case AF_INET6:
			
				pIP = host->h_addr_list;
			for(; *pIP != NULL; pIP++)
				printf("IP address:%s\n", 
				inet_ntop(host->h_addrtype, *pIP, str, sizeof(str)));
               
			    servip= str;
				break;
			default:
				printf("unknown address type\n");
				break;
		}
	}
printf("servip:%s\n",servip);


sockfd=socket( AF_INET,SOCK_STREAM,0);

if (sockfd<0)
{

	printf("creat socket failure: %s\n",strerror(errno));
	return -1;
}
printf("creat socket[%d] scuessfully\n",sockfd);


memset(&servaddr,0,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_port = htons(port);
inet_aton(servip,&servaddr.sin_addr);

endhostent();


rv=connect( sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));

	if(rv<0)
	{	
		printf("connect to server[%s:%d] failure: %s\n",
			servip,port,strerror(errno));
		return -2;
	}
printf("connect to server[%s:%d] successfully\n",servip,port);

while(1)
{

	rv=write(sockfd,MSG_STR,strlen(MSG_STR));

	if (rv<0)
	{	
		printf("write to server by sockfd[%d] failure:%s\n",sockfd,strerror(errno));
		return -4;
	}
	memset(buf,0,sizeof(buf));
	rv=read(sockfd,buf,sizeof(buf));
	if (rv<0)
	{   
		printf("read date from  server by sockfd[%d] failure:%s\n",sockfd,strerror(errno));
		return -5;
	}	  
	else if(rv==0)
	{
		printf("socket[%d] get disconnected\n",sockfd);
		break;
	}
	else if(rv>0)
	{
		printf("Read %d bytes data from server:%s\n",rv,buf);
		break;
	}
}
close(sockfd);

}
