#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>

//#define SERVER_IP		"127.0.0.1"				                //服务器ip
//#define SERVER_PORT		8889					            //服务器端口
#define MSG_STR			"Hello, Unix Network Program World!"	//要发送的数据

void print_usage(char *progname)
{
    printf("%s usage: \n", progname);
    printf("-i(--ipaddr): sepcify server IP address.\n");
    printf("-p(--port): sepcify server port.\n");
    printf("-h(--help): print help informatiom.\n");

    return ;
}

int main(int argc, char **argv)
{
	int		        	 conn_fd = -1;			//客户端描述符
	int			         rv = -1;				//返回值
	char			     buf[1024];				//数据缓冲区
	struct sockaddr_in	 serv_addr;				//服务器地址类型
    char                *serv_ip;
    int                  serv_port;
    struct option        opts[] = {
        {"ipaddr", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    
    int                  ch;

    while( (ch=getopt_long(argc, argv, "i:p:h", opts, NULL)) != -1)
    {
            switch( ch )
            {
                case 'i':
                    serv_ip = optarg;
                    break;
                case 'p':
                    serv_port = atoi(optarg);
                    break;
                case 'h':
                    print_usage(argv[0]);
                    return 0;
            }
    }

    if( !serv_ip || !serv_port )
    {
        print_usage(argv[0]);
        return 0;        
    }

	/*socket函数，创建一个socket（套接字：ip+端口）*/
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);			//网络协议族，传输协议，具体协议
	if( conn_fd < 0 )
	{
		printf("create socket failure: %s\n", strerror(errno));
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));		//使用serv_addr结构体前，先置0
	serv_addr.sin_family = AF_INET;				    //IPV4
	serv_addr.sin_port = htons(serv_port);			//htons将端口号字节序强制转换为网络字节序（小端模式）
	inet_aton( serv_ip, &serv_addr.sin_addr );		//htonl将IP地址强制转换成long类型

	/*connect函数，客户端向服务端发起连接请求*/
	if( connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )	//客户端socket，服务器socket所在
											                                        //大小
	{
		printf("connect to server [%s:%d] failure: %s\n", serv_ip, serv_port, strerror(errno));
		return 0;
	}

	/*write函数，向服务器（写入）发送数据*/
	if( write(conn_fd, MSG_STR, strlen(MSG_STR)) < 0 )
	{
		printf("Write data to server [%s:%d] failure: %s\n", serv_ip, serv_port, strerror(errno));
		goto cleanup;
	}

	memset(buf, 0, sizeof(buf));	//使用buf前，先置为0
					
	/*read函数，读取服务器发来的数据*/
	rv = read(conn_fd, buf, sizeof(buf));
	if(rv < 0)
	{
		printf("Read data from server failure: %s\n", strerror(errno));
		goto cleanup;
	}
/*关闭客户端socket*/
cleanup:
	close(conn_fd);
}



