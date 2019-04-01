/*****************************************************************************
​ ​* ​ ​ @file​ ​  		remoteTask.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to remoteTask.c
 *   @Comm Type 	I2C
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "../inc/message.h"

#define	PORT	(8800)

int new_socket_FD;

int init_socket(void);
int send_data(mesg_t *message);
int read_data(mesg_t *message);
