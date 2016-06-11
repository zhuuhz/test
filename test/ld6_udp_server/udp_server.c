#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include"udp_server.h"


struct sockaddr_in clientAddr;
int sock;
int len;
char buff[512];
int udp_recv_flag = 0;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//thread for send data with udpsever
void *udp_handle(void *arg)
{
    int n;
	while(1)
	{
		// get the client addr, so must need the follow lines
	    n = recvfrom(sock, buff, 511, 0, (struct sockaddr*)&clientAddr, &len);
		if(n <0 ){
			perror("recv err");
			break;
		}

	    udp_recv_flag = 1;
		if(n == 26){
			ConfigRadar(buff);
		}
	}
}

int udp_server_init()
{

	pthread_t idUdp;
	int error;

    printf("Welcome! This is a UDP server, I can only received message from client and reply with same message\n");
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2040);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    len = sizeof(clientAddr);

    //create a pthread of udp, send radar data with udp server
	error=pthread_create(&idUdp, NULL, (void *) udp_handle, NULL);
	if (error!=0)
	{
		printf ("Create uart pthread error!/n");
		return 1;
	}

    while (1)
    {
    	printf("line%d\n",__LINE__);

    	//get radar data and processed it
        dataCanToSerial();
    }

	if ((error = pthread_join (idUdp, NULL)) != 0)
	{
		fprintf (stderr, "udp pthread_join: %s\n", strerror (error));
		return -1;
	}

    return 0;
}
