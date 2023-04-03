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
    printf("-p(--port): sepcify server listen port.\n");
    printf("-h(--help): print help informatiom.\n");

    return ;
}

int main(int argc, char **argv)
{
	int		        	 sock_fd = -1;			//客户端描述符
	int		        	 clie_fd = -1;			//客户端描述符
	int			         rv = -1;				//返回值
	char			     buf[1024];				//数据缓冲区
	struct sockaddr_in	 serv_addr;				//服务器地址类型
	struct sockaddr_in	 clie_addr;				//服务器地址类型
    socklen_t            len;
    int                  serv_port;
    int                  on = 1;
    int                  ch;
    struct option        opts[] = {
        {"port", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    
    while( (ch=getopt_long(argc, argv, "p:h", opts, NULL)) != -1)
    {
            switch( ch )
            {
                case 'p':
                    serv_port = atoi(optarg);
                    break;
                case 'h':
                    print_usage(argv[0]);
                    return 0;
            }
    }

    if( !serv_port )
    {
        print_usage(argv[0]);
        return 0;        
    }
 
	/*socket函数，创建一个socket（套接字：ip+端口）*/
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);			//网络协议族，传输协议，具体协议
	if( sock_fd < 0 )
	{
		printf("Create socket failure: %s!\n", strerror(errno));
		return -1;
	}
	printf("Create socket [%d] successfully.\n", sock_fd);

    /*释放端口*/
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	memset(&serv_addr, 0, sizeof(serv_addr));		//使用serv_addr结构体前，先置0
	serv_addr.sin_family = AF_INET;				    //IPV4
	serv_addr.sin_port = htons(serv_port);			//htons将端口号字节序强制转换为网络字节序（小端模式）
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//htons将端口号字节序强制转换为网络字节序（小端模式）
//	inet_aton( serv_ip, &serv_addr.sin_addr );		//htonl将IP地址强制转换成long类型

	/*bind函数*/
    if(	(rv = bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0 )	//客户端socket，服务器socket所在					                                                                   //大小
	{
		printf("Socket [%d] bind on port [%d] failure: %s!\n", sock_fd, serv_port, strerror(errno));
		return -2;
	}

    listen(sock_fd, 13);
    printf("Listening port [%d]...\n", serv_port);

    while( 1 )
    {
       printf("Waitting for new client to connect...\n");

       if( (clie_fd = accept(sock_fd, (struct sockaddr *)&clie_addr, &len)) < 0 )
        {
            printf("Accept new client failure: %s!\n", strerror(errno));
            continue; 
        }
        printf("Accept new client [%s:%d] successfully.\n", inet_ntoa(clie_addr.sin_addr), ntohs(clie_addr.sin_port));
    
    	/*read函数，读取客户端发来的数据*/
    	memset(buf, 0, sizeof(buf));	//使用buf前，先置为0
    	rv = read(clie_fd, buf, sizeof(buf));
    	if(rv < 0)
    	{
    		printf("Read data from client failure: %s!\n", strerror(errno));
//	    	goto cleanup;
            continue;
    	}
        else if( rv == 0 )
        {
    		printf("Socket [%d] disconnected.\n", clie_fd);
//            goto cleanup;
    		continue;
        }
        else if( rv > 0 )
        {
            printf("Read %d bytes data from client: %s\n", rv, buf);
//            goto cleanup;
            continue;
        }

    	/*write函数，向客户端（写入）发送数据*/
    	if( write(clie_fd, MSG_STR, strlen(MSG_STR)) < 0 )
    	{
    		printf("Write data to client by [%d] failure: %s!\n", clie_fd, strerror(errno));
//    		goto cleanup;
            continue;
    	}
    }
    printf("Close client socket [%d]\n.", clie_fd);
    close(clie_fd);

/*关闭客户端socket*/
//cleanup:
//	close(clie_fd);
}



