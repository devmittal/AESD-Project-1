/*****************************************************************************
​ ​* ​ ​ @file​ ​  		logger.c
​ * ​ ​ @brief​ ​ 		Function definitions for logger.h.
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
 *   @Modified		March 24th, 2019
​ * ​ ​ @version​ ​ 		1.1
*****************************************************************************/

#include"../inc/logger.h"
#include "../inc/message.h"

int write_log(int IsFileCreated, char * LogFilePath)
{
	int value = 0;
	FILE *log_file_ptr;
	mesg_t message;

	uint8_t size = LOGGR_QSIZE;

	value = recv_Message(LOGGR_QNAME, LOGGR_QSIZE, &message);
	if(value == -1)
	{
		return -1;
	}

	struct timeval Now;

	if(!IsFileCreated)
	{
		log_file_ptr = fopen(LogFilePath,"w");
		if(log_file_ptr == NULL)
		{
			printf("PID = %d failed to open file in Write mode\n", getpid());
			exit(0);
		}
		printf("PID = %d opened file in Write mode\n", getpid());
	}
	else
	{
		log_file_ptr = fopen(LogFilePath,"a");
		if(log_file_ptr == NULL)
		{
			printf("PID = %d failed to open file in Append mode\n", getpid());
			exit(0);
		}
		printf("PID = %d opened file in Append mode\n", getpid());
	}

	gettimeofday(&Now,NULL);
	fprintf(log_file_ptr, "[%lu.%06lu] %s \n", Now.tv_sec,Now.tv_usec,message.str);	
	fclose(log_file_ptr);
}

/*
int main(void)
{
	FILE *FP;
	mqd_t loggerFD;
	int n;
	message_struct message;
	struct timeval timestamp;

	FP = fopen(FILENAME,"w");
	if(FP == NULL)
	{
		perror("File could not be created/opened: ");
		exit(1);
	}	

	fprintf(FP,"Project Log File");
	gettimeofday(&timestamp,NULL);
	fprintf(FP,"\n\n[%lu seconds %lu microseconds] BIST: Logger thread has started and is running", timestamp.tv_sec,
			 timestamp.tv_usec);
	fflush(FP);

	loggerFD = mq_open(QNAME, O_RDONLY | O_CREAT, 0666, NULL);
	if(loggerFD == -1)
	{
		perror("Queue not created");
		exit(0);
	}

	while(1)
	{
		n = mq_receive(loggerFD,(char *) &message,8192,NULL);
		if(n == -1)
		{
			perror("Receiving failed");
			exit(0);
		}

		if(message.id == 1)
		{
			if(message.status == 1)
				printf("\n Temperature Sensor Failed");
			else
			{					
				gettimeofday(&timestamp,NULL);
				fprintf(FP,"\n\n[%lu seconds %lu microseconds] Logger Source ID: %s | %f C | %f F | %f K",
						 timestamp.tv_sec, timestamp.tv_usec, message.msg, message.tempC, message.tempF, message.tempK);
				fflush(FP);
			}
		}
	}

	if(mq_close(loggerFD) == -1)
	{
		perror("Closing file failed: ");
		exit(0);
	}

	if(mq_unlink(QNAME) == -1)
	{
		perror("Unlinking file failed: ");
		exit(0);	
	}

	return 0;
}*/