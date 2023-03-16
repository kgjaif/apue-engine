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
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <netdb.h>


void print_usage(char *program)
{
	printf("%s usage :\n",program);
	printf("-i(--ipaddr): sepcify server IP address\n");
	printf("-p(--port): sepcify server \n");
	printf("-i(--help): print this help information\n");
	printf("-d(--domain):sepcify server domain\n");
	return ;
}
float get_temperature(float *temper);

int main(int argc, char **argv)
{

	int 				sockfd=-1;
	int 				rv=-1;
	struct sockaddr_in 	servaddr;
	char				*servip;
	int					port;
	char				buf[1024];
	int 				ch;
	char				buf1[512];
	float       		temper;
	struct	hostent		*host;
	char				str[32];
	char				*domain=NULL;
	char				**pIP=NULL;
	struct option		opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument ,NULL,'h'},
		{"domain",required_argument,NULL,'d'},
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
			case 'h':
				print_usage(argv[0]);
			case 'd':
				domain=optarg;
				break;
				return 0;
		}
	}
	if(!(!servip^!domain)|!port)
	{
		print_usage(argv[0]);
		return 0;
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
		return -2;
	}
	printf("creat socket[%d] scuessfully\n",sockfd);


	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port = htons(port);
	inet_aton(servip,&servaddr.sin_addr);
	rv=connect( sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr));

	if(rv<0)
	{
		printf("connect to server[%s:%d] failure: %s\n",
				servip,port,strerror(errno));
		return -3;
	}
	printf("connect to server[%s:%d] successfully\n",servip,port);

	while(1)
	{
		memset(buf1,0,sizeof(buf1));
		get_temperature(&temper);
		snprintf(buf1,sizeof(buf1),"%f",temper);
		printf("buf1:%s\n",buf1);
		rv=write(sockfd,buf1,strlen(buf1));

		if (rv<0)
		{	
			printf("write to server by sockfd[%d] failure:%s\n",
					sockfd,strerror(errno));
			return -4;
		}
		memset(buf,0,sizeof(buf));
		rv=read(sockfd,buf,sizeof(buf));
		if (rv<0)
		{   
			printf("read date from  server by sockfd[%d] failure:%s\n",
					sockfd,strerror(errno));
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


float get_temperature(float *temper)
{
	int             fd=-1;
	char            buf[128];
	char            *ptr=NULL;
	DIR             *dirp=NULL;
	char            w1_path[64]="/sys/bus/w1/devices/";
	struct dirent   *direntp=NULL;

	char            chip_sn[32];

	char            ds18b20_path[64];

	int             found=0;


	dirp=opendir(w1_path);

	if(!dirp)

	{   
		printf("open folser %s failure:%s\n",w1_path,strerror(errno));

		return -1; 

	}   

	while(NULL !=(direntp=readdir(dirp)))

	{

		if(strstr(direntp->d_name,"28-"))

		{

			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));

			found=1;

		}
	}


	closedir(dirp);

	if(!found)

	{

		printf("can not find de18b20 chipset\n");

		return -2;

	}

	//snprintf(ds18b20_path,sizeof(ds18b20_path),"%s/%s/w1_slave",w1_path,chip_sn);

	//printf("w1_path: %s\n",w1_path);
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));

	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	//printf("w1_path: %s\n",w1_path);


	if((  fd=open(w1_path,O_RDONLY))<0)
	{
		printf("Open file failure:%s\n",strerror(errno));
		return -1;
	}

	memset(buf,0,sizeof(buf));
	if(read(fd,buf,sizeof(buf))<0)
	{
		printf("read data from fd=%d failure:%s\n",fd,strerror(errno));
		return -2;
	}

	//printf("buf:%s\n",buf);
	ptr =strstr(buf,"t=");
	if(!ptr)
	{
		printf("can not find t= string\n");
		return -1;
	}
	ptr+=2;
	//      printf("ptr:%s\n",ptr);
	atof(ptr);
	*temper=atof(ptr)/1000;
	//printf("temper is: %f\n",*temper);
	write(fd,buf,sizeof(buf));
	close(fd);
	return     0;
}
