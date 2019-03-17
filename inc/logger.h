#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<mqueue.h>
#include <sys/time.h>
#include<stdlib.h>

#define FILENAME "logfile"
#define QNAME "/loggerQ"

typedef struct
{
	int id;
	int isFailure;
	float tempC;
	float tempF;
	float tempK;
	char msg[100];
}message_struct;