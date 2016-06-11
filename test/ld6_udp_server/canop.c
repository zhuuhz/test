#include <stdio.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h> 
#include <sys/times.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h> 
#include <signal.h>
#include <limits.h>   
#include "serialport.h"

#include <sys/time.h> //+
#include <math.h>  //+

#include  <errno.h>
#include  <sys/mman.h>
#include <sys/msg.h>

#include "can.h"
#include "canop.h"

#define BUFLEN 1024
void ConfigRadar(char *data);
//============================================================================
int PortOpen()
{
	int fd;
        
    fd = open("/dev/can1", O_RDWR);
	if(fd < 0)
	{
			perror("Open can err");
	}
    return (fd);
}
//======================
void PortClose()
{
   if (close(fd_CAN) == -1)
   	close(fd_CAN);
}

void can_close()
{
	PortClose();
}


//============================================================================
void write_data0(char *da)
{
	canmsg_t tx;
	int  n,i;
	int  fd;

    for(i=0;i < 8;i++)
        tx.data[i] = da[i];
    tx.flags = 0;  
    tx.length = sizeof(tx.data);
    tx.id = 0x200;

    if(write(fd_CAN, &tx, 1) > 0)
    {
		int send_num1 = sendto(sock, "config radar ok", strlen("config radar ok"), 0, (struct sockaddr *)&clientAddr, len);
		if(send_num1 < 0){
			printf("sendto error");
//					return 1;
		}
    #if P
    	 printf("CAN  TX_frame[%3d] > >:  %+3x  %+2x  %+2x  %+2x  %+2x  %+2x  %+2x  %+2x\n", \
		 tx.id, tx.data[0],tx.data[1],tx.data[2],tx.data[3],\
                    tx.data[4],tx.data[5],tx.data[6],tx.data[7]);
	#endif
     //    bus_run_wrong(CAN,fd_CAN);//####
    }
	//bus_run_wrong_masked(CAN,fd_CAN);
  
}
//=====================



char data[100];
int dataFuc(canmsgs_t msgs)
{	
	canmsg_t rx;
	int i = 0;
	int send_flag = 0;
	for (i = 0; i < msgs.mnum; ++i)
	{
		rx = msgs.mmsg[i];

		memset(data, 0, 100);
		if (rx.id == 0x60b)       
		{		   
			n60c = 0;
			measStat = (rx.data[7]>>6)&0x3;

			if (measStat != 0)
			{
			 id = (rx.data[0]>>2)&0x3f;				

			 longDispl = (((unsigned int)rx.data[2]>>5)&0x07) | (((unsigned int)rx.data[1]<<3)&0x7f8); 		

			 latDispl = ((rx.data[6]>>6)&0x03) | (((unsigned int)rx.data[5]<<2)&0x3fc);				

			 dynProp = (rx.data[6]>>3)&0x7;				

			 width = (rx.data[7]>>3)&0x7;				

			 length = rx.data[7]&0x7;				

			 probOfExist = rx.data[6]&0x7;			

			 vrelLong = 
				(((unsigned int)rx.data[3]>>1)&0x7f) | (((unsigned int)rx.data[2]<<7)&0xf80);								
			}        
		} 	 
		else if (rx.id == 0x60c)       
		{

			if ((n60c == 0) && (measStat != 0))
			{
				unsigned int latSpeed = rx.data[1];		
			

				unsigned int jy = id + longDispl + latDispl + dynProp + measStat + length + width + probOfExist + vrelLong + latSpeed;
				jy = jy%0x100;	
				sprintf(data,"&%03x%02x%03x%03x%01x%01x%01x%01x%01x%03x%03x%02x|", 0x200, id,longDispl, latDispl, dynProp, measStat,length,width,probOfExist,vrelLong,latSpeed,jy);		

				//send data with udpserver
				if(udp_recv_flag == 1)
				{
					int send_num1 = sendto(sock, data, strlen(data), 0, (struct sockaddr *)&clientAddr, len);
					if(send_num1 < 0){
						printf("sendto error in %s,%d line\n"__FILE__, __LINE__);
						return 1;
					}
				}
				//send data to serial port
				serial_send(data,strlen(data));
			} 
			n60c++;
		}		
		else if (rx.id == 0x201)     
		{          		
			unsigned int power = (rx.data[0]>>7)&0x1;
			unsigned int elevation = rx.data[3];
			unsigned int rangellen = rx.data[4];
			
			unsigned int jy = power + elevation + rangellen + 1 + 1;
			jy = jy%0x100;
			
			sprintf(data,"&%03x%02x%03x%03x%01x%01x000000000%02x|", 0x100,power,elevation,rangellen,1,1,jy);

			if(udp_recv_flag == 1){
				int send_num2 = sendto(sock, data, strlen(data), 0, (struct sockaddr *)&clientAddr, len);
				if(send_num2 < 0){
					printf("sendto error in %s,%d line\n"__FILE__, __LINE__);
					return 1;
				}
			}

			//send data to serial port
			serial_send(data,strlen(data));
			printf("%s\n", data);

//			printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", rx.data[0], rx.data[1], rx.data[2], rx.data[3], rx.data[4], rx.data[5], rx.data[6], rx.data[7]);
		}                
		else{}
		}
	return 0;
}

void ConfigRadar(char *data)
{
	char buf[BUFLEN];
	memset(buf, 0, sizeof(buf));
	strncpy(buf,data,strlen(data));
	int num = strlen(buf);
	if (num > 25)
	{
		unsigned char da[8];
		unsigned char da1[16];
		memset(da, 0, sizeof(da));
		if ((num == 26) && (buf[25] == '|'))
		{
			if ((buf[0] == '&') && (buf[1] == '3') && (buf[2] == '3') && (buf[3] == '3'))
			{

				unsigned int jl,jd;
				jd = (buf[4]-'0')*10 + buf[5] - '0';
				jl = (buf[6]-'0')*100 + (buf[7]-'0')*10 + buf[8] - '0';

				sprintf(da1, "0f%02x%02x0200000000", jl, jd);
				CharToHex(da, da1, 16);
				//#if P
				printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", da[0],da[1],da[2],da[3],da[4],da[5],da[6],da[7]);
				//#endif

				write_data0(da);
			}
		}
	}
}

//============================================================================
void read_data0(void)
{
	#if P
		printf ("CAN read\n");
	#endif

	canmsgs_t msgs;
	/*	unsigned char da[8];
		unsigned char da1[16];
		sprintf(da1, "0f644200000000");
		CharToHex(da, da1, 16);
		printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", da[0],da[1],da[2],da[3],da[4],da[5],da[6],da[7]);
		write_data0(da);
	 */
	int num ;
	char buf[BUFLEN];
    for(;;)
    {
    	//receive config rada data with serialport
    	num = serial_receive(buf, 1024);
    	if(num > 25){
    		ConfigRadar(buf);
    		memset(buf, 0, sizeof(buf));
    	}
		if((msgs.mnum = read(fd_CAN, msgs.mmsg, 83))>0 ){
			if(dataFuc(msgs) == 1)
			{
				break;
			}
		}
    }
}
//====================
 
//============================================================================
int PortSet()
{
	op_AccMask();
	op_AccCode();
	op_Baud   ();
	op_set_mode();
	return 0 ;
}
//============================================================================
void op_AccMask()
{
	int fd;

	fd = fd_CAN;
    cfg.target = CONF_ACCM;
    cfg.val1   = (unsigned long)T_accmask;
    ioctl(fd, CONFIG, &cfg);
}
//----------------------
void op_AccCode()
{
	int fd;
	fd = fd_CAN;
    cfg.target = CONF_ACCC;
    cfg.val1   = (unsigned long)T_acccode;
    ioctl(fd, CONFIG, &cfg);
}
//----------------------
void op_Baud()
{
	int fd;
    fd = fd_CAN;
    cfg.target = CONF_TIMING;
    cfg.val1   = (int)T_baud;
    ioctl(fd, CONFIG, &cfg);
}
//----------------------
void op_set_mode()
{
	int temp=T_mode_reg ,fd;   ////@
    fd = fd_CAN;
    cfg.target = CONF_MODE;//
    cfg.val1   = (int)temp;
    ioctl(fd, CONFIG, &cfg);  //val1=1 set; val1=0 reset
}
//============================================================================
void op_Sleep_mode()
{
	printf("sleep mode\n");
	int temp=1,fd;
    fd = fd_CAN;
    cfg.target = CONF_SLEEP_MODE;//
    cfg.val1   = (int)temp;
    ioctl(fd, CONFIG, &cfg);  //val1=1 set; val1=0 reset
}
//----------------------
void op_Set_filter()
{
	printf("filter mode\n");
	int temp=1,fd;
    fd = fd_CAN;
    cfg.target = CONF_FILTER_MODE;
    cfg.val1   = (unsigned long)temp;  //##
    ioctl(fd, CONFIG, &cfg);//
}
//----------------------
void op_Self_test_mode()
{
	printf("self test mode\n");
	int temp=1,fd;
    fd = fd_CAN;
    cfg.target = CONF_SELF_TEST_MODE;//
    cfg.val1   = (int)temp;
    ioctl(fd, CONFIG, &cfg);
	printf("done\n");
}
//----------------------
void op_ListenOnly(int port)
{
	printf("litsen only mode\n");
	int temp=1,fd;
	fd = fd_CAN;
    cfg.target = CONF_LISTEN_ONLY_MODE;
    cfg.val1   = (int)temp;
    ioctl(fd, CONFIG, &cfg);  //val1=1 set; val1=0 reset
}
//=========================================================================================

void bus_run_wrong_masked(int can_num,int fd)
{
unsigned char  status_v,errcode_v,irq_value_v;
CanStatusPar_t stat;
        ioctl(fd, STATUS, &stat);  //$$
sleep(0.01);
        status_v    = (unsigned char)stat.status;
        errcode_v   = (unsigned char)stat.error_code;
		irq_value_v = (unsigned char)stat.irq_value;

	if((errcode_v != old_errcode)&&(status_v!=old_status))
	{
if(irq_value_v & 0x80 )       printf("##[CAN %d]Bus Error!(0x%x)\n",can_num+1,errcode_v);
else if(irq_value_v & 0x08)   printf("##[CAN %d]Data Overrun!(0x%x)\n",can_num+1,errcode_v);

	old_errcode = errcode_v;
	old_status  = status_v;
	}
}

//=========================================================================================
void bus_run_wrong(int can_num,int fd)
{
unsigned char  status_v,errcode_v,irq_value_v;
CanStatusPar_t stat;
        ioctl(fd, STATUS, &stat);  //$$
sleep(0.01);
        status_v  = (unsigned char)stat.status;
        errcode_v = (unsigned char)stat.error_code;
		irq_value_v = (unsigned char)stat.irq_value;

if(irq_value_v & 0x80 )       printf("##[CAN %d]Bus Error!(0x%x)\n",can_num+1,errcode_v);
else if(irq_value_v & 0x08)   printf("##[CAN %d]Data Overrun!(0x%x)\n",can_num+1,errcode_v);

}
//=========================================================================================
int str2int(char para[10])
{
char num[10],tt[10];
int i,j,n;
int total;

for(i=0;i<10;i++) num[i]=para[i];
i=strlen(num);
if(i<8)   {for(j=7;j>=i;j--) tt[j]=0;}
for(j=i-1;j>=0;j--)
        {
        if((num[j]>='0') && (num[j])<= '9') tt[i-j-1]=num[j]-'0';
        if((num[j]>='a') && (num[j])<= 'f') tt[i-j-1]=num[j]-'a'+10;
        if((num[j]>='A') && (num[j])<= 'F') tt[i-j-1]=num[j]-'A'+10;
        }
total=tt[7]*(16*16*16*16*16*16*16)+\
      tt[6]*(16*16*16*16*16*16)+\
      tt[5]*(16*16*16*16*16)+\
      tt[4]*(16*16*16*16)+\
      tt[3]*(16*16*16)+\
      tt[2]*(16*16)+\
      tt[1]*16+\
      tt[0] ;
return total;
}
//=========================================================================================

void dataCanToSerial()
{

	 read_data0();    
}
//=========================================================================================

void can_init(void)
{
	T_baud	  = 500;
	T_accmask = 0xffffffff;
	T_acccode = 0xffffffff;
	T_mode_reg= 0x08;
	
	 
	fd_CAN = PortOpen(); ///
	if (fd_CAN < 0) 
	{
		fd_CAN = PortOpen();
	}
	PortSet();	 ///
	PortClose(); //+
	fd_CAN = PortOpen(); //+
	if (fd_CAN <= 0) 
	{
		fd_CAN = PortOpen();
	}

}

void CharToHex(unsigned char* buf,char*str,int len)
{
    int i=0;
    int j=0;
    unsigned char temp;
    while(i<len)
    {
        temp=str[i];
        if((temp>='0')&&(temp<='9'))
        {
            temp = temp-'0';
            buf[j]=temp<<4;
        }
        else if((temp>='a')&&(temp<='f'))
        {
            temp=temp-'a'+0x0a;
            buf[j]=temp<<4;
        }
        else
        {
            buf[j]=0x00;
        }

        temp=str[i+1];
        if((temp>='0')&&(temp<='9'))
        {
            temp = temp-'0';
            buf[j]=buf[j]|temp;
        }
        else if((temp>='a')&&(temp<='f'))
        {
            temp = temp-'a'+0x0A;
            buf[j]=buf[j]|temp;
        }
        else
        {
            buf[j]=buf[j]|0x00;
        }
        j = j+1;
        i = i+2;
    }
    return;
}


