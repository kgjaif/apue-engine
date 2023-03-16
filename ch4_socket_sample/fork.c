/*********************************************************************************
 *      Copyright:  (C) 2022 lanli
 *                  All rights reserved.
 *
 *       Filename:  fork.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/02/22)
 *         Author:  lanli
 *      ChangeLog:  1, Release initial version on "11/02/22 07:14:23"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc,char **argv)
{
	pid_t			pid;
	printf("Parent process PID[%d] stat running..\n ", getpid());
	pid =fork();
	if(pid<0)
	{
		printf("fork() create child process failure:%s\n",strerror(errno));
	}
	else if (pid==0)
	{
		printf("Child process PID [%d] start running,my parent PID is [%d]\n ",getpid(),getppid());
		return 0;
	}
	else //if(pid>0)
	{
		printf("parent process PID[%d] continue running , and child process PID is [%d]\n",getpid(), pid );
	
		return 0;
	}


}

