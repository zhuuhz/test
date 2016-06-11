/*-----------------------------------------------------
*NAME:  rfid.c
*----------------------------------------
*copyright(c)
*--------------------------------------------
*RELEASE:
*REVISION:
*---------------------------------------------
*PURPOSE:
*这个文件是对系统中的串口进行配置和启动关闭 控制函数
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



/******打开、设置串口*******/
void serial_init(void)
{
   	
	  /*************打开串口**************/
	 fd = open("/dev/ttyS0",O_RDWR | O_NOCTTY | O_NDELAY );
     if( fd == -1)
     {
            perror("can't open serial port");
            return;
     }
	fcntl(fd,F_SETFL,0);
	tcgetattr(fd,&termios_fd);
	
   /***************设置串口波特率115200************/ 
   struct termios opt;
   tcgetattr(fd,&opt);
   tcflush(fd,TCIOFLUSH);
   cfsetispeed(&opt,B115200);
   cfsetospeed(&opt,B115200);
   tcflush(fd,TCIOFLUSH);
   tcsetattr(fd, TCSANOW, &opt);          
   tcflush(fd,TCIOFLUSH);

   /***************设置串口数据位8、流控无、停止位1、非阻塞************/ 
   struct termios options;
   if(tcgetattr( fd, &options ) !=0)
           {
       perror("setup Serial error!");
       return;
           }


   options.c_cflag |= CLOCAL | CREAD;
   options.c_cflag &= ~CSIZE;

   //数据位8
   options.c_cflag |= CS8;
        
	//无流控
   options.c_cflag &= ~PARENB;
   options.c_iflag &= ~INPCK;

   //停止位1
   options.c_cflag &= ~CSTOPB;
   
  //采用非标准阻塞模式
   options.c_lflag &= ~(ICANON | ECHO |ECHOE |ISIG);          
   options.c_oflag &= ~OPOST;
   options.c_iflag &= ~(IXON | IXOFF | IXANY);
   options.c_iflag &= ~(INLCR | IGNCR | ICRNL);
   options.c_oflag &= ~(ONLCR | OCRNL);

   options.c_cc[VTIME] = 0;
   options.c_cc[VMIN] = 0;
           
/**************************
1．当VTIME>0，VMIN>0时。read调用将保持阻塞直到读取到第一个字符，
读到了第一个字符之后开始计时，此后若时间到了VTIME或者时间未到但
已读够了VMIN个字符则会返回；若在时间未到之前又读到了一个字符
(但此时读到的总数仍不够VMIN)则计时重新开始。
2. 当VTIME>0，VMIN=0时。read调用读到数据则立即返回，否则将为每个字符最多等待VTIME时间。
3. 当VTIME=0，VMIN>0时。read调用一直阻塞，直到读到VMIN个字符后立即返回。
4. 若在open或fcntl设置了O_NDELALY或O_NONBLOCK标志，read调用不会阻塞而是立即返回，那么VTIME和VMIN就没有意义，效果等同于与把VTIME和VMIN都设为了0。
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
