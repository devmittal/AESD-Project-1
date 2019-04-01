/*****************************************************************************
 *  @File         i2c.h
 *  @Brief        Contains function declarations pertaining to i2c.c
 *  @Tools_Used   ARM-LINUX-GCC
 *  @Author(s)    Souvik De, Devansh Mittal
 *  @Date         March 24th, 2019
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
#define CONFIG_REG_TEMP 	(0x01)

sem_t i2c_bus_lock; 		/* Semaphore Variable - Initialized and Destroyed in Main*/
int I2C_FileDescriptor;		/* I2C File Descriptor */

/**
* @Brief  This function initializes the I2C bus
* @Param  DeviceID: Sensor address
* @Return Error code
**/
int init_i2c(uint8_t DeviceID);

/**
* @Brief  This function writes 8-bit data to the I2C Bus
* @Param  (1) I2C file descriptor; (2) Registor address to be written to.
* @Return Error code
**/
int write_i2c(int fd, uint8_t register_type);

/**
* @Brief  This function writes 16-bit data to the I2C Bus
* @Param  (1) I2C file descriptor; (2) Registor address to be written to; (3) Command register
*		  address.
* @Return Error code
**/
int write_i2c16(int fd, uint16_t register_type,uint8_t command_register_addr);

/**
* @Brief  This function reads 16-bit data from the I2C Bus
* @Param  I2C file descriptor
* @Return Address of the variable that holds the 16-bit value
**/
uint8_t* read_i2c16(int fd);

/**
* @Brief  This function reads 8-bit data from the I2C Bus
* @Param  I2C file descriptor
* @Return Data to be returned / Error Code in case of failure
**/
int read_i2c8(int fd);

/**
* @Brief  This function concludes the I2C Communication.
* @Param  I2C file descriptor
* @Return Error code
**/
int close_i2c(int fd);

/**
* @Brief  This function writes 24-bit data to the I2C Bus to write to the configuration register
* 		  of the temperature sensor.
* @Param  (1) I2C file descriptor; (2) Registor data to be write.
* @Return Error code
**/
int write_i2c16_config(int fd, uint16_t register_type);

#endif
