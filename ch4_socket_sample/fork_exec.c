/*********************************************************************************
 *      Copyright:  (C) 2022 Zhu Lijun
 *                  All rights reserved.
 *
 *       Filename:  fork_exec.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/02/22)
 *         Author:  Zhu Lijun <3262465970@qq.com>
 *      ChangeLog:  1, Release initial version on "11/02/22 10:17:19"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define TMP_FILE		"/tmp/.ifconfig.log "

int main(int argc,char **argv)
{
	int 	fd=-1;
	pid_t	pid;
	char	buf[1024];
	char	rv;
	char	*ptr;
	FILE	*fp;
	char	*ip_start;
	char	*ip_end;
	char	ipaddr[16];



	if((fd=open(TMP_FILE,O_RDWR|O_CREAT|O_TRUNC<0644))<0)
			{
				printf("Redirect standard output to file failure :%s\n",strerror(errno));
				return -1;
			}
	pid =vfork();
	if(pid<0)
	{
		printf("fork() creat child process failure:%s\n",strerror(errno));
		return -1;
	}
	else if(pid==0)
	{
		printf("Child process start excute ifconfig program\n");
		dup2(fd,STDOUT_FILENO);
		execl("/sbin/ifconfig","ifconfig","wlan0",NULL);
		printf("Child process excute another program,willl not return here.Return here means execl() error\n");
		return -1;
	}
	else
	{
		sleep(3);
	}

	memset(buf,0,sizeof(buf));
	lseek(fd,0,SEEK_SET);
	rv=read(fd,buf,sizeof(buf));
	printf("Read %d bytes datd after lseek :\n %s",rv,buf);
	
	fp=fdopen(fd,"r");
	fseek(fp,0,SEEK_SET);
	while(fgets(buf,sizeof(buf),fp))
			{
			if(strstr(buf,"netmask"))
				{
				ptr=strstr(buf,"inet");
				if(!ptr)
					{
						break;
					}
				ptr+=strlen("inet");
				while(isblank(*ptr))
					ptr++;
					ip_start=ptr;
				while(!isblank(*ptr))
					ptr++;
					ip_end =ptr;
				memset(ipaddr,0,sizeof(ipaddr));
				memcpy(ipaddr,ip_start,ip_end-ip_start);

				break;


				}

			}

	printf("Parser and get IP address:%s\n",ipaddr);

	fclose(fp);
	unlink(TMP_FILE);
	return 0;


}
