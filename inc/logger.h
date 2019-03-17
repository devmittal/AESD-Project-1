/*****************************************************************************
​ ​* ​ ​ @file​ ​  		logger.c
​ * ​ ​ @brief​ ​ 		Header file for Logger thread containing defines and 
 *					structures
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

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
	int status;
	float tempC;
	float tempF;
	float tempK;
	char msg[100];
}message_struct;