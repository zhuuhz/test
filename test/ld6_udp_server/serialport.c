/*-----------------------------------------------------
*NAME:  rfid.c
*----------------------------------------
*copyright(c)
*--------------------------------------------
*RELEASE:
*REVISION:
*---------------------------------------------
*PURPOSE:
*����ļ��Ƕ�ϵͳ�еĴ��ڽ������ú������ر� ���ƺ���
-----------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "serialport.h"


#define Flag    1
#define gprsFlag   2

struct termios termios_fd;
int fd;

char serial_send(unsigned char *ptr, unsigned int len)
{
    int tmplen;
    if( (tmplen=write(fd, ptr, len)) < 0 )
    {
       	perror("serial send error");
    } 
	return tmplen;
}

int serial_receive(char *buf, int len)
{
    int tmplen=0;
  	if((tmplen = read(fd, buf, len))<0)
    {
        	perror("serial read  error\n");
			tcsetattr(fd,TCSANOW,&termios_fd);
			close(fd);
			serial_init();
			return 0;
  	}
    return tmplen; 
}



/******�򿪡����ô���*******/
void serial_init(void)
{
   	
	  /*************�򿪴���**************/
	 fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY | O_NDELAY );
     if( fd == -1)
     {
            perror("can't open serial port");
            return;
     }
	fcntl(fd,F_SETFL,0);
	tcgetattr(fd,&termios_fd);
	
   /***************���ô��ڲ�����115200************/ 
   struct termios opt;
   tcgetattr(fd,&opt);
   tcflush(fd,TCIOFLUSH);
   cfsetispeed(&opt,B115200);
   cfsetospeed(&opt,B115200);
   tcflush(fd,TCIOFLUSH);
   tcsetattr(fd, TCSANOW, &opt);          
   tcflush(fd,TCIOFLUSH);

   /***************���ô�������λ8�������ޡ�ֹͣλ1��������************/ 
   struct termios options;
   if(tcgetattr( fd, &options ) !=0)
           {
       perror("setup Serial error!");
       return;
           }


   options.c_cflag |= CLOCAL | CREAD;
   options.c_cflag &= ~CSIZE;

   //����λ8
   options.c_cflag |= CS8;
        
	//������
   options.c_cflag &= ~PARENB;
   options.c_iflag &= ~INPCK;

   //ֹͣλ1
   options.c_cflag &= ~CSTOPB;
   
  //���÷Ǳ�׼����ģʽ
   options.c_lflag &= ~(ICANON | ECHO |ECHOE |ISIG);          
   options.c_oflag &= ~OPOST;
   options.c_iflag &= ~(IXON | IXOFF | IXANY);
   options.c_iflag &= ~(INLCR | IGNCR | ICRNL);
   options.c_oflag &= ~(ONLCR | OCRNL);

   options.c_cc[VTIME] = 0;
   options.c_cc[VMIN] = 0;
           
/**************************
1����VTIME>0��VMIN>0ʱ��read���ý���������ֱ����ȡ����һ���ַ���
�����˵�һ���ַ�֮��ʼ��ʱ���˺���ʱ�䵽��VTIME����ʱ��δ����
�Ѷ�����VMIN���ַ���᷵�أ�����ʱ��δ��֮ǰ�ֶ�����һ���ַ�
(����ʱ�����������Բ���VMIN)���ʱ���¿�ʼ��
2. ��VTIME>0��VMIN=0ʱ��read���ö����������������أ�����Ϊÿ���ַ����ȴ�VTIMEʱ�䡣
3. ��VTIME=0��VMIN>0ʱ��read����һֱ������ֱ������VMIN���ַ����������ء�
4. ����open��fcntl������O_NDELALY��O_NONBLOCK��־��read���ò������������������أ���ôVTIME��VMIN��û�����壬Ч����ͬ�����VTIME��VMIN����Ϊ��0��
*****************************/
   tcflush(fd,TCIFLUSH);

   if((tcsetattr(fd,TCSANOW,&options))!=0)
   {
       perror("serial set error");
       return;
   }
}

void serial_close()
{
	close(fd);
}
