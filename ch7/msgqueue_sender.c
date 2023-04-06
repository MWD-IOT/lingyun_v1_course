/*********************************************************************************
 *      Copyright:  (C) 2023 NULL
 *                  All rights reserved.
 *
 *       Filename:  msgqueue_sender.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年04月06日)
 *         Author:  MoWeida <2594041017@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年04月06日 21时00分34秒"
 *                 
 ********************************************************************************/



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FTOK_PATH		"/dev/zero"
#define FTOK_PROJID		0x22

typedef struct s_msgbuf
{
	long		mtype;
	char		mtext[512];
}t_msgbuf;

int main(int argc, char **argv)
{
	int				i;
	int				msgid;
	int				msgtype;
	key_t			key;
	t_msgbuf		msgbuf;

	if( (key=ftok(FTOK_PATH, FTOK_PROJID)) < 0 )
	{
		printf("Ftok() get IPC token failure: %s!\n", strerror(errno));
		return -2;
	}
	
	msgid = msgget(key, IPC_CREAT|0666);
	if( msgid < 0 )
	{
		printf("Shmget() create shared memory failure: %s!\n", strerror(errno));
		return -2;
	}

	msgtype = (int)key;
	printf("Key[%d] msgid[%d] msgtype[%d].\n", (int)key, msgid, msgtype);

	for(i=0; i<100; i++)
	{
		msgbuf.mtype = msgtype;
		strcpy(msgbuf.mtext, "Ping");
		if( msgsnd(msgid, &msgbuf, sizeof(msgbuf.mtext), IPC_NOWAIT) < 0 )
		{
			printf("Msgsnd() send message failure: %s.\n", strerror(errno));
			break;
		}
		printf("Send message %d : %s %d\n", i, msgbuf.mtext, i);
		sleep(1);
	}
	printf("Receive %d messages.\n", i);
	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}



