/*********************************************************************************
 *      Copyright:  (C) 2023 NULL
 *                  All rights reserved.
 *
 *       Filename:  named_socket_server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2023年04月05日)
 *         Author:  MoWeida <2594041017@qq.com>
 *      ChangeLog:  1, Release initial version on "2023年04月05日 14时10分20秒"
 *                 
 ********************************************************************************/



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket.domain"

int main(int argc, char **argv)
{
	int						rv = -1;
	int						listen_fd;
	int						client_fd = -1;
	char					buf[1024];
	struct sockaddr_un		serv_addr;
	struct sockaddr_un		cli_addr;
	socklen_t				cliaddr_len = sizeof(struct sockaddr); 
	
	listen_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if( listen_fd < 0 )
	{
		printf("Create socket failure: %s!\n", strerror(errno));
		return -1;
	}
	printf("Socket create fd[%d] successfully.\n", listen_fd);

	if( !access(SOCKET_PATH, F_OK) )
	{
		unlink(SOCKET_PATH);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

	if( bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
	{
		printf("Create socket failure: %s!\n", strerror(errno));
		unlink(SOCKET_PATH);
		return -2;
	}
	printf("Socket[%d] bind on path \"%s\" successfully.\n", listen_fd, SOCKET_PATH);

	listen(listen_fd, 13);

	while( 1 )
	{
		printf("\nStart waiting and accept new client connect...\n");
		client_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cliaddr_len);
		printf("client_fd: %d\n", client_fd);
		if( client_fd < 0 )
		{
			printf("Accept new socket failure: %s!\n", strerror(errno));
			return -2;
		}

		memset(buf, 0, sizeof(buf));
		rv = read(client_fd, buf, sizeof(buf));
		printf("rv: %d\n", rv);
		if( rv < 0 )
		{
			printf("Read data from client socket[%d] failure: %s!\n", client_fd, strerror(errno));
			close(client_fd);
			continue;
		}
		else if( rv == 0 )
		{
			printf("Client socket[%d] disconnected.\n", client_fd);
			close(client_fd);
			continue;
		}
		printf("Read %d bytes data from client[%d] and echo it back:\n%s\n", rv, client_fd, buf);
		
		if( write(client_fd, buf, rv) < 0 )
		{
			printf("Write %d bytes data back to client[%d] failure: %s!\n", rv, client_fd, strerror(errno));
			close(client_fd);
		}
		sleep(1);
		close(client_fd);
	}
	close(listen_fd);
}



