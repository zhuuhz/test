/*
 * tcp.h
 *
 *  Created on: 2016年4月7日
 *      Author: yhx
 */

#ifndef TCP_H_
#define TCP_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>



#include<pthread.h>
#include<unistd.h>

#include <sys/stat.h>



extern void dataCanToSerial();

#endif /* TCP_H_ */
