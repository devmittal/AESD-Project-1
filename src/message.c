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

void init_MessageQueues(void)
{
	isKillSignal = 0;
	main_queue_fd = open_MessageQueue(MAIN_QNAME, MAIN_QSIZE);
	logger_queue_fd = open_MessageQueue(LOGGR_QNAME, LOGGR_QSIZE);
	tempt_queue_fd = open_MessageQueue(TEMPT_QNAME, TEMPT_QSIZE);
	light_queue_fd = open_MessageQueue(LIGHT_QNAME, LIGHT_QSIZE);
}

mqd_t open_MessageQueue(char *QueueName, uint8_t QueueSize)
{
	mqd_t fd;
	struct mq_attr fd_attr;

	fd_attr.mq_maxmsg = QueueSize;
	fd_attr.mq_msgsize = sizeof(mesg_t);

	fd = mq_open(QueueName, O_CREAT | O_RDWR, 0666, &fd_attr);

	return fd;
}

void send_Message(char *QueueName, uint8_t priority, mesg_t* message)
{
	int value = 0;

	if(strcmp(QueueName, LOGGR_QNAME) == 0)
	{
		value = mq_send(logger_queue_fd, (char *)message, sizeof(mesg_t), priority);
		if(value == -1)
		{
			perror("Failed to send message through logger message queue ");
			exit(-1);	
		}
	}
	else if(strcmp(QueueName, MAIN_QNAME) == 0)
	{
		value = mq_send(main_queue_fd, (char *)message, sizeof(mesg_t), priority);	
		if(value == -1)
		{
			perror("Failed to send message through main message queue ");
			exit(-1);	
		}
	}
	else if(strcmp(QueueName, TEMPT_QNAME) == 0)
	{
		value = mq_send(tempt_queue_fd, (char *)message, sizeof(mesg_t), priority);	
		if(value == -1)
		{
			perror("Failed to send message through temperature message queue ");
			exit(-1);	
		}
	}
	else if(strcmp(QueueName, LIGHT_QNAME) == 0)
	{
		value = mq_send(light_queue_fd, (char *)message, sizeof(mesg_t), priority);	
		if(value == -1)
		{
			perror("Failed to send message through light message queue ");
			exit(-1);	
		}
	}
	else	
	{
		printf("\nInvalid Queue Name in send !\n");
		exit(-1);
	}
}

int recv_Message(char *QueueName, uint8_t *priority, mesg_t* message)
{
	int value = 0;
	unsigned int prio;

	if(strcmp(QueueName, LOGGR_QNAME) == 0)
	{
		value = mq_receive(logger_queue_fd, (char *)message, sizeof(mesg_t), &prio);
		if(value == -1)
		{
			perror("Failed to receive message through logger message queue ");
			return value;	
		}
		*priority = prio;
	}
	else if(strcmp(QueueName, MAIN_QNAME) == 0)
	{
		value = mq_receive(main_queue_fd, (char *)message, sizeof(mesg_t), &prio);
		if(value == -1)
		{
			if(!isKillSignal)
				perror("Failed to receive message through main message queue ");
			
			return value;	
		}
		*priority = prio;
	}
	else if(strcmp(QueueName, TEMPT_QNAME) == 0)
	{
		value = mq_receive(tempt_queue_fd, (char *)message, sizeof(mesg_t), &prio);
		if(value == -1)
		{
			perror("Failed to receive message through temperature message queue ");
			return value;	
		}
		*priority = prio;
	}
	else if(strcmp(QueueName, LIGHT_QNAME) == 0)
	{
		value = mq_receive(light_queue_fd, (char *)message, sizeof(mesg_t), &prio);
		if(value == -1)
		{
			perror("Failed to receive message through light message queue ");
			return value;	
		}
		*priority = prio;
	}
	else
	{
		printf("\nInvalid Queue Name in receive !\n");
		return -1;
	}

	return 0;
}

void CloseUnlinkQueue(mqd_t fd, char* QueueName)
{
	mq_close(fd);
	mq_unlink(QueueName);	
}

void dest_MessageQueues(void)
{
	CloseUnlinkQueue(main_queue_fd, MAIN_QNAME);
	CloseUnlinkQueue(logger_queue_fd, LOGGR_QNAME);
	CloseUnlinkQueue(tempt_queue_fd, TEMPT_QNAME);
	CloseUnlinkQueue(light_queue_fd, LIGHT_QNAME);
}
