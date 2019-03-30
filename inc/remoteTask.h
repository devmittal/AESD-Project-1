/*****************************************************************************
​ ​* ​ ​ @file​ ​  		remoteTask.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to remoteTask.c
 *   @Comm Type 	I2C
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORTNO 5555

void init_socket(void);