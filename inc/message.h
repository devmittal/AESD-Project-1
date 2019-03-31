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
#include <signal.h>
#include <errno.h>
#include "temperature.h"
#include "lightSensor.h"

#define TEMPT_QNAME	("/tempt_queue")
#define LIGHT_QNAME	("/light_queue")
#define	LOGGR_QNAME	("/logger_queue")
#define	MAIN_QNAME	("/main_queue")
#define	TEMPT_QSIZE	(8)
#define	LIGHT_QSIZE	(8)
#define	LOGGR_QSIZE	(8)
#define	MAIN_QSIZE	(8)

typedef struct Message
{
	tempt_t temperature;
	light_t light;
	int IsLoggerError;
	int IsRemoteError;
	char str[100];
}mesg_t;

mqd_t logger_queue_fd;
mqd_t main_queue_fd;
mqd_t light_queue_fd;
mqd_t tempt_queue_fd;
int isKillSignal;

/* 3rd element is included to send only message without any data */
enum Message_Priority{PRIO_TEMPERATURE, PRIO_LIGHT, PRIO_LOG, PRIO_NODATA, PRIO_REMOTE};

void init_MessageQueues(void);
void dest_MessageQueues(void);
mqd_t open_MessageQueue_NonBlocking(char *QueueName, uint8_t QueueSize);
mqd_t open_MessageQueue(char *QueueName, uint8_t QueueSize);
void send_Message(char *QueueName, uint8_t priority, mesg_t* message);
int recv_Message(char *QueueName, uint8_t *priority, mesg_t* message);
void CloseUnlinkQueue(mqd_t fd, char* QueueName);

#endif