/*****************************************************************************
 *   @File  		message.h
 *   @Brief		Contains dependecies pertaining to message.c, which enab-
 *			les message passing between different thread (IPC) using
 *			Message Queues.
 *   @Comm Type		I2C
 *   @Tools_Used 	ARM-LINUX-GCC
 *   @Author(s)		Souvik De, Devansh Mittal
 *   @Date		March 24th, 2019
 *   @version		1.0
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

#define TEMPT_QNAME	("/tempt_queue")	//Temperature message queue name
#define LIGHT_QNAME	("/light_queue")	//Light message queue name
#define	LOGGR_QNAME	("/logger_queue")	//Logger message queue name
#define	MAIN_QNAME	("/main_queue")		//Main message queue name
#define	TEMPT_QSIZE	(8)					//Temperature message queue size				
#define	LIGHT_QSIZE	(8)					//Light message queue size
#define	LOGGR_QSIZE	(8)					//Logger message queue size
#define	MAIN_QSIZE	(8)					//Main message queue name

/* Message structure used to exchange messages throught message queues and socket */
typedef struct Message
{
	tempt_t temperature;
	light_t light;
	int IsLoggerError;
	int IsRemoteError;
	char str[100];
	char loglevel[20];
}mesg_t;

mqd_t logger_queue_fd;		//Logger queue file descriptor
mqd_t main_queue_fd;		//Main queue file descriptor
mqd_t light_queue_fd;		//Light queue file descriptor
mqd_t tempt_queue_fd;		//Temperature queue file descriptor
int isKillSignal;			//Kill notifier

/* Message queue priorities for different threads*/
/* 3rd element is included to send only message without any data */
enum Message_Priority{PRIO_TEMPERATURE, PRIO_LIGHT, PRIO_LOG, PRIO_NODATA, PRIO_REMOTE};

/**
* @Brief  This function initializes all the message queues
* @Param  void
* @Return void
**/
void init_MessageQueues(void);

/**
* @Brief  This function destroyes (closes and unlinks) all the message queues
* @Param  void
* @Return void
**/
void dest_MessageQueues(void);

/**
* @Brief  This function opens a message queue identified by its parameters
* @Param  (1)Message Queue Name; (2)Message Queue Size
* @Return Message Queue file descriptor
**/
mqd_t open_MessageQueue(char *QueueName, uint8_t QueueSize);

/**
* @Brief  This function is used to send messages through a queue identified by the first parameter 
* @Param  (1)Message Queue Name; (2)Message Queue Priority; (3) Message to be sent
* @Return void
**/
void send_Message(char *QueueName, uint8_t priority, mesg_t* message);

/**
* @Brief  This function is used to receive messages through a queue identified by the first parameter 
* @Param  (1)Message Queue Name; (2)Message Queue Priority; (3) Message being received
* @Return Error code
**/
int recv_Message(char *QueueName, uint8_t *priority, mesg_t* message);

/**
* @Brief  This function closes and unlinks a message queue identified by the first parameter
* @Param  (1) Message Queue file descriptor; (2) Message Queue Name
* @Return void
**/
void CloseUnlinkQueue(mqd_t fd, char* QueueName);

#endif
