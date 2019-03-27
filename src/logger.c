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
	uint8_t queue_priority;
	FILE *log_file_ptr;
	mesg_t message;

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

		gettimeofday(&Now,NULL);
		fprintf(log_file_ptr, "[%lu.%06lu] Logfile Create by Logger Thread (Thread ID  = %lu) \n", Now.tv_sec,Now.tv_usec,syscall(__NR_gettid));	
	}
	else
	{
		log_file_ptr = fopen(LogFilePath,"a");
		if(log_file_ptr == NULL)
		{
			printf("\nPID = %d failed to open file in Append mode\n", getpid());
			exit(0);
		}
		printf("PID = %d opened file in Append mode\n", getpid());

		value = recv_Message(LOGGR_QNAME, LOGGR_QSIZE,  &queue_priority, &message);
		if(value == -1)
		{
			perror("Logger Failed to receive message through message queue :");
			return -1;
		}

		gettimeofday(&Now,NULL);
		fprintf(log_file_ptr, "[%lu.%06lu] %s \n", Now.tv_sec,Now.tv_usec, message.str);

		if(queue_priority == PRIO_TEMPERATURE)
		{
			fprintf(log_file_ptr, "\t|\n");
			fprintf(log_file_ptr, "\t--------------->In Celcius : %f\n",message.temperature.celcius);
			fprintf(log_file_ptr, "\t\t|\n");
			fprintf(log_file_ptr, "\t\t--------------->In Farenheit : %f\n",message.temperature.farenheit);
			fprintf(log_file_ptr, "\t\t\t|\n");
			fprintf(log_file_ptr, "\t\t\t--------------->In Kelvin : %f\n",message.temperature.kelvin);
		}
		else
		{
			fprintf(log_file_ptr, "\t|\n");
			fprintf(log_file_ptr, "\t--------------->LUX Visible Light : %d\n",message.light.lux_visiblelight);
			fprintf(log_file_ptr, "\t\t|\n");
			fprintf(log_file_ptr, "\t\t--------------->LUX Infrared Light : %d\n",message.light.lux_irlight);
			fprintf(log_file_ptr, "\t\t\t|\n");
			fprintf(log_file_ptr, "\t\t\t--------------->Lumens : %f\n",message.light.lumen);
		}
	}

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