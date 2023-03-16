/*********************************************************************************
 *      Copyright:  (C) 2023 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  signal.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/25/23)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "02/25/23 04:01:17"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int g_child_stop =0;
int g_parent_run =0;

void sig_child(int signum)
{
	if(SIGUSR1==signum)
	{
		g_child_stop=1;
	}

}

void sig_parent(int signum)
{
	if(SIGUSR2==signum)
	{
		g_parent_run=1;
	}
}


int main(int argc, char **argv)
{
	int      	pid;
	int			wstatus;

	signal(SIGUSR1,sig_child);
	signal(SIGUSR2,sig_parent);

if(pid=fork()<0)
{

	printf("Creat child process failure: %s\n",strerror(errno));

	 return -2;
}
else if(pid==0)
{
	printf("Child process start running and send parent a signal\n");
	kill (getppid(),SIGUSR2);
	while (!g_child_stop)
	{
		sleep(1);
		
	}
	printf("Child process receive signal from parent and exit now\n");
	return 0;
}
printf("Parent hangs up untill receive signal from child!\n");
while(!g_parent_run)
{
	sleep(1);
}

printf("Parent start running now and send child a signal to exit\n");
kill(pid,SIGUSR1);

wait(&wstatus);
printf("Parent wait child process die and exit now\n");

return 0;


}




