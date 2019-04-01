/*****************************************************************************
 *   @File        remoteTask.h
 *   @Brief       Contains dependencies pertaining to remoteTask.c
 *   @Comm Type   I2C
 *   @Tools_Used  Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
 *   @Author(s)   Souvik De, Devansh Mittal
 *   @Date        March 30th, 2019
 *   @version     1.0
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "../inc/message.h"

#define	PORT	(8800)	//Port Number

/* Socket side socket file descriptor for client */
int new_socket_FD;

/**
* @Brief  This function initializes the socket at the server side
* @Param  void
* @Return Error code
**/
int init_socket(void);

/**
* @Brief  This function sends data through a socket from server to client
* @Param  Message structure variable to be sent
* @Return Error code
**/
int send_data(mesg_t *message);

/**
* @Brief  This function reads data sent from client to server via a socket
* @Param  Message structure variable received from client
* @Return Error code
**/
int read_data(mesg_t *message);
