/*********************************************************************************
 *      Copyright:  (C) 2022 iot<iot@email.com>
 *                  All rights reserved.
 *
 *       Filename:  signal.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/10/22)
 *         Author:  iot <iot@email.com>
 *      ChangeLog:  1, Release initial version on "11/10/22 07:07:49"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <execinfo.h>


int 		g_sigstop=0;

void signal_stop(int signum)
{
	if(SIGTERM==signum)
	{
		printf("SIGTERM signal detected\n");
	}
	else if (SIGALRM==signum)
	{
		printf("SIGALRM signal detected\n");
		g_sigstop =1;
	}
}

void signal_user(int signum)
{
	if (SIGUSR1==signum)
	{
		printf("SIGUSR1 signal detected\n");
	}
	else if (SIGUSR2==signum)
	{
		printf("SIGUSER2 signal detected\n");
	}
	g_sigstop=1;
}

void signal_code(int signum)
{
	if(SIGBUS == signum)
	{
		printf("SIGBUS signal detected\n");
	}
	else if(SIGILL == signum)
	{
		printf("SIGILL signal detected\n");
	}
	else if(SIGSEGV == signum)
	{
		printf("SIGSEGV signal detected\n");
	}

	exit(-1);
}




int main(int argc,char **argv)
{
	char *ptr=NULL;
	struct sigaction sigact, sigign;
	/* +--------------------------------------+
	 * *| Method1: Use signal() install signal |
	 * *+--------------------------------------+*/

	signal(SIGTERM, signal_stop);
	signal(SIGALRM, signal_stop);

	signal(SIGBUS, signal_code);
	signal(SIGILL, signal_code);
	signal(SIGSEGV, signal_code);

	/* +-----------------------------------------+
	 *| Method2: Use sigaction() install signal |
	 *+-----------------------------------------+*/

	/*  Initialize the catch signal structure. */
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = signal_user;

	/*  Setup the ignore signal. */
	sigemptyset(&sigign.sa_mask);
	sigign.sa_flags = 0;
	sigign.sa_handler = SIG_IGN;

	sigaction(SIGINT, &sigign, 0); /*  ignore SIGINT signal by CTRL+C */

	sigaction(SIGUSR1, &sigact, 0); /*  catch SIGUSR1 */
	sigaction(SIGUSR2, &sigact, 0); /*  catch SIGUSR1 */


	printf("Program start running for 5 seconds...\n");
	alarm(5);

	while( !g_sigstop )
	{ 
		;
	}

	printf("Program start stop running...\n");

	printf("Invalid pointer operator will raise SIGSEGV signal\n");
	*ptr = 'h';

	return 0;




}

