/*********************************************************************************
 *      Copyright:  (C) 2022 Zhu Lijun
 *                  All rights reserved.
 *
 *       Filename:  socket_server.c
 *    Description:  This file i
 *                 
 *        Version:  1.0.0(11/02/22)
 *         Author:  Zhu Lijun <3262465970@qq.com>
 *      ChangeLog:  1, Release initial version on "11/02/22 14:47:47"
 *                 
 ********************************************************************************/
#include <sys/types.h>    
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#define MSG_STR "Hello world client"


void print_usage(char *program)
{
	printf("%s usage :\n",program);
	printf("-i(--ipaddr): sepcify server IP address\n");
	printf("-p(--port): sepcify server listen port.\n");
	printf("-i(--help): print this help information\n");

	return ;
}


int main(int argc, char **argv)
{

	int                 sockfd=-1;
	int                 clifd=-1;
	int                 rv=-1;
	socklen_t           len;
	struct sockaddr_in  servaddr;
	struct sockaddr_in  cliaddr;
	char                *servip;
	int                 port;
	char                buf[1024];
	int                 ch; 
	int                 on=1;
	pid_t				pid;
	struct option       opts[]={
		{"port",required_argument,NULL,'p'},
		{"help",no_argument ,NULL,'h'},
		{NULL,0,NULL,0}
	};  
	printf("start parser arguments\n");


	while ((ch=getopt_long(argc,argv,"p:h",opts,NULL))!=-1)
	{   
		printf("ch:%c\n",ch);
		switch(ch){


			case 'p':
				port=atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}
	if(!port)
	{
		print_usage(argv[0]);
		return 0;
	}


	sockfd=socket( AF_INET,SOCK_STREAM,0);

	if (sockfd<0)
	{

		printf("creat socket failure: %s\n",strerror(errno));
		return -1;
	}
	printf("creat socket[%d] scuessfully\n",sockfd);

	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	//inet_aton(servip,&servaddr.sin_addr);


	rv=bind(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));

	if(rv<0)
	{
		printf("socket[%d] bind on port[%d] failure: %s\n",
				sockfd,port,strerror(errno));
		return -2;
	}

	listen(sockfd,13);

	while(1)
	{

		printf("start to listen on port[%d] \n",port);
		clifd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
		if(clifd<0)
		{
			printf("Accept new client failure :%S\n",strerror(errno));
			continue;
		}
		printf("Accept new client[%s:%d] successfully\n", inet_ntoa(cliaddr.sin_addr),
				ntohs(cliaddr.sin_port));
		pid = fork();
		if( pid < 0 )
		{
			printf("fork() create child process failure: %s\n", strerror(errno));
			close(clifd);
			continue;
		}
		else if( pid > 0 )
		{
			/*  Parent process close client fd and goes to accept new socket client again */
			close(clifd);
			continue;
		}
		else if ( 0 == pid )
		{
			char buf[1024];
			int i;
			printf("Child process start to commuicate with socket client...\n");
			close(sockfd); /*  Child process close the listen socket fd */
			while(1)
			{
				memset(buf, 0, sizeof(buf));
				rv=read(clifd, buf, sizeof(buf));
				if( rv < 0 )
				{
					printf("Read data from client sockfd[%d] failure: %s\n", 	                   clifd,strerror(errno));
					close(clifd);
					exit(0);
				}
				else if( rv == 0) 
				{
					printf("Socket[%d] get disconnected\n", clifd);
					close(clifd);
					exit(0);
				}
				else if( rv > 0 )
				{
					printf("Read %d bytes data from Server: %s\n", rv, buf);
				}
				/*  convert letter from lowercase to uppercase */
				for(i=0; i<rv; i++)
				{
					buf[i]=toupper(buf[i]);
				}
				rv=write(clifd, buf, rv);
				if(rv < 0)
				{
					printf("Write to client by sockfd[%d] failure: %s\n", clifd,
							strerror(errno));
					close(clifd);
					exit(0);
				}
			} /*  Child process loop */
		} /*  Child process start*/
	}
	close(sockfd);
	return 0;


}

