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
	mesg_t message;

	struct timeval Now;

	if(!IsFileCreated)
	{
		log_file_ptr = fopen(LogFilePath,"w");
		if(log_file_ptr == NULL)
		{
			printf("PID = %d failed to open file in Write mode\n", getpid());
			return -1;
		}
		printf("PID = %d opened file in Write mode\n", getpid());

		gettimeofday(&Now,NULL);
		fprintf(log_file_ptr, "[%lu.%06lu] INFO : Logfile Created by Logger Thread. Startup Test successful (Thread ID  = %lu). \n"
				, Now.tv_sec,Now.tv_usec,syscall(__NR_gettid));	
	}
	else
	{
		log_file_ptr = fopen(LogFilePath,"a");
		if(log_file_ptr == NULL)
		{
			printf("\nPID = %d failed to open file in Append mode\n", getpid());
			return -1;
		}
		printf("PID = %d opened file in Append mode\n", getpid());

		if(recv_Message(LOGGR_QNAME, &queue_priority, &message) < 0)
		{
			fclose(log_file_ptr);
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
		else if(queue_priority == PRIO_LIGHT)
		{
			fprintf(log_file_ptr, "\t|\n");
			if(message.light.isLight)
				fprintf(log_file_ptr, "\t--------------->State : Light\n");
			else
				fprintf(log_file_ptr, "\t--------------->State : Dark\n");
			fprintf(log_file_ptr, "\t\t|\n");
			fprintf(log_file_ptr, "\t\t--------------->LUX Visible Light : %d\n",message.light.lux_visiblelight);
			fprintf(log_file_ptr, "\t\t\t|\n");
			fprintf(log_file_ptr, "\t\t\t--------------->LUX Infrared Light : %d\n",message.light.lux_irlight);
			fprintf(log_file_ptr, "\t\t\t\t|\n");
			fprintf(log_file_ptr, "\t\t\t\t--------------->Lumens : %f\n",message.light.lumen);
			fflush(log_file_ptr);
		}
	}

	fclose(log_file_ptr);

	return 0;
}