/*
 * client
 */
#include"client.h"

#define MAXLINE 4096
int sockfd;
int client_init(int port, char* ip_adress)
{
    int     n;
    int connect_ok = 0;
    char    recvline[4096], sendline[4096];
    struct sockaddr_in    servaddr;

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if( inet_pton(AF_INET, ip_adress, &servaddr.sin_addr) <= 0){
		printf("inet_pton error for %s\n",ip_adress);
		exit(0);
    }

    if( (connect_ok = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
		printf("cnnnect_ok = %d\n",connect_ok );
//		exit(0);
		if(connect_ok == -1)
		{
			printf("the client is waiting connect to server , please look the server is opened.");
//			connect_ok = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
//			sleep(1);
		}
    }

//    printf("send msg to server: ");
//	  fflush(stdout);
//    fgets(sendline, 4096, stdin);
//    if( send(sockfd, sendline, strlen(sendline), 0) < 0)
//    {
//    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
//    exit(0);
//    }

//    close(sockfd);
//    exit(0);
}
