#include"../inc/logger.h"

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
			if(message.isFailure == 1)
				printf("\n Temperature Sensor Failed");
			else
			{					
				gettimeofday(&timestamp,NULL);
				fprintf(FP,"\n\n[%lu seconds %lu microseconds] Logger Source ID: \"%s\" | %f C | %f F | %f K",
						 timestamp.tv_sec, timestamp.tv_usec, message.msg, message.tempC, message.tempF, message.tempK);
				fflush(FP);
			}
		}
	}

	return 0;
}