/*********************************************************************************
 *      Copyright:  (C) 2023 NULL
 *                  All rights reserved.
 *
 *       Filename:  thread.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年03月26日)
 *         Author:  MoWeida <2594041017@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年03月26日 20时29分47秒"
 *                 
 ********************************************************************************/



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_worker1(void *args);
void *thread_worker2(void *args);

int main(int argc, char **argv)
{
	int				shared_var = 100;
	pthread_t		tid;  //线程ID
	pthread_attr_t  thread_attr;  //线程属性

	/* 初始化 */
	if( pthread_attr_init(&thread_attr) )
	{
		printf("pthread_attr_init() failure: %s!\n", strerror(errno));
		return -1;
	}

	/* 设置线程堆栈大小 */
	if( pthread_attr_setstacksize(&thread_attr, 120 * 1024) )
	{
		printf("thread_attr_setstacksize() failure: %s!\n", strerror(errno));
		return -1;
	}

	/* 设置线程属性为 分离 */
	if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )
	{
		printf("pthread_attr_setdetachestate() failure: %s!\n", strerror(errno));
		return -1;
	}
	
	pthread_create(&tid, &thread_attr, thread_worker1, &shared_var);  //创建线程1（分离）
	printf("Thread worker1 tid [%ld] created ok.\n", tid);

	pthread_create(&tid, NULL, thread_worker2, &shared_var);  //创建线程2（会合）
	printf("Thread worker2 tid [%ld] created ok.\n", tid);

	pthread_attr_destroy(&thread_attr);  //销毁线程

	/*Wait until thread worker2 exit()*/
	pthread_join(tid, NULL);  //会合

	/* 主线程死循环 */
	while( 1 )
	{
		printf("Main/Control thread shared_var: %d.\n", shared_var);
		sleep(10);
	}
}

	/* 任务1 */
	void *thread_worker1(void *args)
	{
		int			*ptr = (int *)args;

		if( !args )
		{
			printf("%s() get invalid arguments.\n", __FUNCTION__);
			pthread_exit(NULL);
		}

		printf("Thread worker 1 [%ld] start running...\n", pthread_self());

		while( 1 )
		{
			printf("+++: %s before shared_var++: %d.\n", __FUNCTION__, *ptr);
			*ptr += 1;
			sleep(2);
			printf("+++; %s after sleep shared_var: %d.\n", __FUNCTION__, *ptr);
		}
		printf("Tr=hread worker 1 exit...\n");

		return NULL;
	}

	/* 任务2 */
	void *thread_worker2(void *args)
	{
		int			*ptr = (int *)args;

		if( !args )
		{
			printf("%s() get invalid arguments.\n", __FUNCTION__);
			pthread_exit(NULL);
		}

		printf("Thread worker 1 [%ld] start running...\n", pthread_self());

		while( 1 )
		{
			printf("---: %s before shared_var++: %d.\n", __FUNCTION__, *ptr);
			*ptr += 1;
			sleep(2);
			printf("---; %s after sleep shared_var: %d.\n", __FUNCTION__, *ptr);
		}

		printf("Tr=hread worker 1 exit...\n");

		return NULL;
	}



