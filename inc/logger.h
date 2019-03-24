/*****************************************************************************
​ ​* ​ ​@file​ ​  		logger.c
​ * ​ ​@brief​ ​ 		Header file for Logger thread containing defines pertain-
 *					to logger.c.
 *  @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​@Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​@Date​ ​​ 			March 16th, 2019
 *	@Modified		March 24th, 2019
​ * ​ ​@version​ ​ 		1.1
*****************************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <sys/time.h>

int write_log(int IsFileCreated, char *LogFilePath);

#endif