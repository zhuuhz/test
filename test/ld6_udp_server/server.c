/*
 * server
 */
#include"tcp.h"

#define MAXLINE 4096

int connfd;
int    listenfd;
/*
 * save message to file
 */
int savemsg(char* msg)
{
    int fd, res;
    fd = open("/mnt/sata/msg.txt",O_RDWR | O_APPEND | O_CREAT);
    if(fd < 0)
    {
        printf("open file error\n");
		return fd;
    }
    res = write(fd, msg, strlen(msg));
    close(fd);
    return res;
}

/*
 * rec socket message
 */
int doit(int confd)
{
	int n = 0;
	char buf[4096];
	while(1)
	{
    	n = recv(confd, buf, MAXLINE, 0);
		if(n > 0)
		{
    		buf[n] = '\0';
    		printf("recv msg from client[%d]: %s\n", n, buf);
    		savemsg(buf);
		}
    	else
    	{
    		break;
    	}
    	usleep(10);
	}
	return n;
}


//void thread_test1()
//{
//	int num;
//	num = connfd;
//	dataCanToSerial();
//	printf("thread_test1 closed file in  %s ,line %d\n", __FILE__, __LINE__);
//	close(num);
//}

int server_init(int port)
{

    struct sockaddr_in     servaddr;
    char    buf[4096];
    int     n;
    int reuseaddr = 1;
	pthread_t idtest1, idtest2;
	int error;
//	int port = 2040;
//	if(argc > 2)
//    {
//        printf("Usage: %s [port]", argv[0]);
//        printf("\n\tdefault port is 5001\n");
//    }
//    if(argc==2)
//        port=atoi(argv[1]);

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    //ip reused
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(reuseaddr)) == -1){
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
    }

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);
    }

    if( listen(listenfd, 10) == -1){
    	printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
    	exit(0);
    }

    while(1){
    	printf("======waiting for client's request======\n");
		printf("port: %d\n", port);
    	if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        	printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        	continue;
    	}

		printf("client come in\n");
		while(1)
		{
			if(send(connfd,"yanghexin",sizeof("yanghexin"),0)<0){
				printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
				break;
			}
			usleep(10);

		}


////		doit(connfd);
//		error = pthread_create(&idtest1, NULL, (void *)thread_test1, NULL);
//		if(error != 0)
//		{
//			printf("Create test1 pthread error!\n");
//			return 1;
//		}

//    	dataCanToSerial();
    	close(connfd);
    }

//	if ((error = pthread_join (idtest1, NULL)) != 0)
//	{
////		fprintf (stderr, "idtest1 pthread_join: %s\n", strerror (error));
//		printf("error:%d\n",error);
//		return -1;
//	}
    close(listenfd);
}
