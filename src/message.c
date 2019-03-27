/*****************************************************************************
 * @File		message.c
 * @Brief		Contains function definitions for message.h.
 * @Comm Type 		I2C
 * @Tools_Used 		ARM-LINUX-GCC
 * @Author(s)		Souvik De, Devansh Mittal
 * @Date 		March 24th, 2019
 * @version 		1.0
*****************************************************************************/

#include "../inc/message.h"

mqd_t open_MessageQueue(char *QueueName, uint8_t QueueSize)
{
	mqd_t fd;
	struct mq_attr fd_attr;

	fd_attr.mq_maxmsg = QueueSize;
	fd_attr.mq_msgsize = sizeof(mesg_t);

	fd = mq_open(QueueName, O_CREAT | O_RDWR, 0666, &fd_attr);

	return fd;
}

int send_Message(char *QueueName, uint8_t QueueSize, uint8_t priority, mesg_t* message)
{
	int value = 0;
	mqd_t fd;

	fd = open_MessageQueue(QueueName, QueueSize);
	if(fd == -1)
	{
		perror("Failed to open message queue");
		exit(-1);
	}

	value = mq_send(fd, (char *)message, sizeof(mesg_t), priority);
	if(value == -1)
	{
		perror("Failed to send message through message queue");
		exit(-1);	
	}

	CloseUnlinkQueue(fd, QueueName);

	return value;
}

int recv_Message(char *QueueName, uint8_t QueueSize, uint8_t *priority, mesg_t* message)
{
	int value = 0;
	unsigned int prio;
	mqd_t fd;

	fd = open_MessageQueue(QueueName, QueueSize);
	if(fd == -1)
	{
		perror("Failed to open message queue");
		exit(-1);
	}

	value = mq_receive(fd, (char *)message, sizeof(mesg_t), &prio);
	*priority = prio;

	CloseUnlinkQueue(fd, QueueName);

	return value;
}

void CloseUnlinkQueue(mqd_t fd, char* QueueName)
{
	mq_close(fd);
	mq_unlink(QueueName);	
}
