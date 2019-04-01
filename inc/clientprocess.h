/*****************************************************************************
​ ​* ​ ​ @file​ ​  		clientprocess.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to clientprocess.c
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../inc/message.h"

#define	PORT	(8800)	//Port Number

/* Client side socket file descriptor */
int socket_FD;

/**
* @Brief  This function initializes the socket at the client side
* @Param  void
* @Return Error code
**/
int init_socket(void);

/**
* @Brief  This function sends data through a socket from client to server
* @Param  Message structure variable to be sent
* @Return Error code
**/
int send_data(mesg_t *message);

/**
* @Brief  This function reads data sent from server to client via a socket
* @Param  Message structure variable received from server
* @Return Error code
**/
int read_data(mesg_t *message);