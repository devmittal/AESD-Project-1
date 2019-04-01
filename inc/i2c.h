/*****************************************************************************
 *  @File         i2c.h
 *  @Brief        Contains function declarations pertaining to i2c.c
 *  @Tools_Used   ARM-LINUX-GCC
 *  @Author(s)    Souvik De, Devansh Mittal
 *	@Date         March 24th, 2019
 *  @version      1.0
*****************************************************************************/

#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define TMP102_DEV_ID		(0x48) //Temperature sensor address
#define APDS_9301_DEV_ID	(0x39) //Light sensor address

sem_t i2c_bus_lock; /*Initialized and Destroyed in Main*/
int I2C_FileDescriptor;

/**
* @Brief  This function initializes i2c communication
* @Param  DeviceID: Sensor address
* @Return 
**/
int init_i2c(uint8_t DeviceID);
int write_i2c(int fd, uint8_t register_type);
int write_i2c16(int fd, uint16_t register_type);
uint8_t* read_i2c16(int fd);
int read_i2c8(int fd);
int close_i2c(int fd);

#endif
