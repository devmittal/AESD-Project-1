/*****************************************************************************
​ ​* ​ ​ @file​ ​  		temperature.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to temperature.c and
 *					the temperature sensor TMP102.
 *	 @Comm Type 	I2C
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define TMP102_DEV_ID	(0x48)

int init_temperature(void);
float read_temperature(int fd, int type);

#endif