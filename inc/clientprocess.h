/*****************************************************************************
​ ​* ​ ​ @file​ ​  		clientprocess.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to clientprocess.c
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../inc/message.h"

#define	PORT	(8800)

int socket_FD;

int init_socket(void);
int send_data(mesg_t *message);
int read_data(mesg_t *message);