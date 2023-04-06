/*********************************************************************************
 *      Copyright:  (C) 2023 NULL
 *                  All rights reserved.
 *
 *       Filename:  shared_mem_write.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年04月06日)
 *         Author:  MoWeida <2594041017@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年04月06日 19时41分37秒"
 *                 
 ********************************************************************************/



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FTOK_PATH 			"/dev/zero"
#define FTOK_PROJID 		0x22

typedef struct st_student
{
	int			age;
	char		name[64];
}t_student;

int main(int argc, char **argv)
{
	int			 i;
	int			 shmid;
	key_t		 key;
	t_student	*student;

	if( (key = ftok(FTOK_PATH, FTOK_PROJID)) < 0 )
	{
		printf("Ftok() get IPC token failure: %s!\n", strerror(errno));
		return -1;
	}

	shmid = shmget(key, sizeof(t_student), IPC_CREAT | 0666);
	if( shmid < 0 )
	{
		printf("Shmget() create shared memory failur: %s!\n", strerror(errno));
		return -2;
	}

	student = shmat(shmid, NULL, 0);
	if( ((void *) - 1) == student )
	{
		printf("Shmat() alloc shared memory failure: %s!\n", strerror(errno));
		return -2;
	}

	strncpy(student->name, "zhangsan", sizeof(student->name));
	student->age = 18;

	for( i=0; i<100; i++ )
	{
		student->age ++;
		printf("Student '%s' age [%d].\n", student->name, student->age);
		sleep(1);
	}

	shmdt(student);

	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}




