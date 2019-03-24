/*****************************************************************************
​ ​* ​ ​@file​ ​  		message.h
 *	@brief			Contains dependecies pertaining to message.c, which enab-
 *					les message passing between different thread (IPC) using
 *					Message Queues.
 *	@Comm Type 		I2C
 *  @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​@Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​@Date​ ​​ 			March 24th, 2019
​ * ​ ​@version​ ​ 		1.0
*****************************************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <mqueue.h>
#include <sys/stat.h>
#include "temperature.h"

#define TEMPT_QNAME	("/tempt_queue")
#define LIGHT_QNAME	("/light_queue")
#define	LOGGR_QNAME	("/logger_queue")
#define	TEMPT_QSIZE	(8)
#define	LIGHT_QSIZE	(8)
#define	LOGGR_QSIZE	(8)

typedef struct Message
{
	tempt_t temperature;
	char str[100];
}mesg_t;

mqd_t open_MessageQueue(char *QueueName, uint8_t QueueSize);
int send_Message(mqd_t fd, mesg_t* message);
int recv_Message(mqd_t fd, mesg_t* message);
void CloseUnlinkQueue(mqd_t fd, char* QueueName);

#endif