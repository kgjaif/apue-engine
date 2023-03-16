/*********************************************************************************
 *      Copyright:  (C) 2022 lanli
 *                  All rights reserved.
 *
 *       Filename:  daemon.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/09/22)
 *         Author:  lanli
 *      ChangeLog:  1, Release initial version on "11/09/22 11:13:49"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <libgen.h>
int main(int argc,char **argv)
{
	char *progname = basename(argv[0]);
	
	if(daemon(0,0)<0)
	{
		printf("Progname dameon() failure:%s\n",strerror(errno));
		return -1;
		
	}
	openlog("daemon",LOG_CONS | LOG_PID,0);
	syslog(LOG_NOTICE,"Progname '%s' start running\n",progname);
	syslog(LOG_WARNING, "progname '%s' running with a warning message\n",progname);
	syslog(LOG_EMERG,"progname '%s' runnig with a emergency message\n",progname);
	while(1)
	{
		//Do something here
		;
	}

	syslog(LOG_NOTICE,"Program '%s' stop running\n",progname);
	closelog();
	return 0;
}
