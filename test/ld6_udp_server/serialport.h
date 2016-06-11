/*-------------------------------------------------
*NAME:  serialport.h
*----------------------------------------
*copyright(c)
*--------------------------------------------
*RELEASE:
*REVISION:
*---------------------------------------------
*PURPOSE:
*这个文件给系统串口的配置和启动函数
*-------------------------------------------------------------*/
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

extern char serial_send(unsigned char *ptr, unsigned int len);
extern int serial_receive(char *buf, int len);
extern void serial_init(void);
extern void serial_close(void);

#endif 
