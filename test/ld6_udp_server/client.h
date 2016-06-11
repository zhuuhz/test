/*
 * client.h
 *
 *  Created on: 2016年4月7日
 *      Author: yhx
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
int client_init(int port, char* ip_adress);
#endif /* CLIENT_H_ */
