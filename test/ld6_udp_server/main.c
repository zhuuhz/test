#include <stdio.h>
#include "serialport.h"
#include "can.h"
#include <string.h>

//#include"tcp.h"
#include"client.h"
#include"udp_server.h"
//extern int connfd;
//char *Ip_Adress = "192.168.0.105";
//int port = 2040;




int main(int argc, char* argv[])
{

	serial_init(); //�򿪴��ڡ���ʼ������
	can_init();

//	client_init(2040, Ip_Adress);
	udp_server_init();
//	server_init(port);

//	dataCanToSerial();	   //��can��ȡ���ݽ��д�����ʹ���

	can_close();
	serial_close(); //�رմ���
	return 0;

	
}
