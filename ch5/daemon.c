/*********************************************************************************
 *      Copyright:  (C) 2023 NULL
 *                  All rights reserved.
 *
 *       Filename:  daemon.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年03月26日)
 *         Author:  MoWeida <2594041017@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年03月26日 13时37分16秒"
 *                 
 ********************************************************************************/


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <libgen.h>

int main(int argc, char **argv)
{
	char		*progname = basename(argv[0]);

	if( daemon(0, 0) < 0 )
	{
		printf("Program daemon() failure: %s\n", strerror(errno));
		return -1;
	}

	openlog("daemon", LOG_CONS | LOG_PID, 0);
	syslog(LOG_NOTICE, "Program '%s' start running...\n", progname);
	syslog(LOG_WARNING, "Program '%s' running with a warnning message!\n", progname);
	syslog(LOG_EMERG, "Program '%s' running with a emergency message!\n", progname);

	while( 1 )
	{
		;
	}

	syslog(LOG_NOTICE, "Program '%s' stop running.\n", progname);
	closelog();

	return 0;
}


