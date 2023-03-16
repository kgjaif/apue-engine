/*********************************************************************************
 *      Copyright:  (C) 2022 Zhu Lijun
 *                  All rights reserved.
 *
 *       Filename:  dup_dup2.c
 *    Description:  This file 
 *                 i
 *        Version:  1.0.0(10/25/22)
 *         Author:  Zhu Lijun <3262465970@qq.com>
 *      ChangeLog:  1, Release initial version on "10/25/22 11:17:02"
 *                 
 ********************************************************************************/
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
int main (int argc, char **argv[])
{
	int fd=-1;
	fd=open("std.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
	if (fd<0)
	{
		printf("open file failure:%s\n",strerror(errno));
		return 0;
	}
	dup2(fd,STDIN_FILENO);
	dup2(fd,STDOUT_FILENO);
	dup2(fd,STDERR_FILENO);
	printf("fd=%d\n",fd);
	close(fd);


}







