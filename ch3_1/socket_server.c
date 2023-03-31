#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LISTEN_PORT	8889					//监听端口
#define BACKLOG		13					//内核允许的最大排队连接数

int main(int argc, char **argv)
{
	int			rv = -1; 			//返回值
	int			listen_fd, client_fd = -1;  	//文件描述符
	struct sockaddr_in	serv_addr;  			//服务器地址
	struct sockaddr_in	cli_addr; 			//客户端地址
	socklen_t		cliaddr_len;  			//客户端地址长度
	char			buf[1024];			//数据缓冲区

	/*socket函数，创建一个socket（套接字：ip+端口）*/
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);		//网络协议族，传输协议，具体协议
	if( listen_fd < 0 )
	{
		printf("create socket failure: %s\n", strerror(errno));
		return -1;
	}
	printf("socket create fd[%d]\n", listen_fd);

	memset(&serv_addr, 0, sizeof(serv_addr));		//使用serv_addr结构体前，先置0
	serv_addr.sin_family = AF_INET;				//IPV4
	serv_addr.sin_port = htons(LISTEN_PORT);		//htons将端口号字节序强制转换为网络字节序（小端模式）
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  	//htonl将IP地址强制转换成long类型
	
	/*bind函数，为已创建的socket分配一个具体的地址（绑定）*/
	if( bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
	{
		printf("creat socket failure: %s\n", strerror(errno));
		return -2;
	}
	printf("socket[%d] bind on port[%d] for all IP address ok\n", listen_fd, LISTEN_PORT);

	/*监听已创建的socket，变为被动模式，等待客户端连接*/
	listen(listen_fd, BACKLOG);				//要监听的socket描述符，内核允许的最大排队连接数

	while( 1 )
	{
		printf("\nStart waiting and accept new client connect...\n");

		/*accept函数，为请求连接的客户端创建一个socket并返回给客户端，每个客户端调用一次该函数，创建一个socket*/
		client_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cliaddr_len);	//监听的socket，客户端ip，
												//客户端ip长度
		if( client_fd < 0 )
		{
			printf("accept new socket failure: %s\n", strerror(errno));
			return -2;
		}
		printf("Accept new client[%s:%d] with fd [%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), client_fd);

		memset(buf, 0, sizeof(buf));		//使用buf前，先置为0

		/*read函数，读取客户端发来的数据*/
		if( (rv = read(client_fd, buf, sizeof(buf))) < 0 )
		{
			printf("Read data from client socket[%d] failure: %s\n", client_fd, strerror(errno));
			close(client_fd);
			continue;
		}
		else if( rv == 0 )
		{
			printf("client socket[%d] disconnected\n", client_fd);
			close(client_fd);
			continue;
		}
		printf("read  %d bytes data from client[%d] and echo it back: '%s'\n", rv, client_fd, buf);

		/*write函数，向客户端（写入）发送数据*/
		if( write(client_fd, buf, rv) < 0 )
		{
			printf("Write %d bytes data back to client[%d] failure: %s\n", rv, client_fd, strerror(errno));
			close(client_fd);
		}
		sleep(1);

		/*关闭socket*/
		close(client_fd);
	}

	/*关闭socket*/
	close(client_fd);
}



