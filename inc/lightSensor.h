/*****************************************************************************
​ ​* ​ ​ @file​ ​  		lightSensor.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to lightSensor.c and
 *					the light sensor APDS-9301.
 *					Note: Addresses prepended with "CMD" indicates a write to
 *					the command register
 *	 @Comm Type 	I2C
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 24th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include<linux/i2c-dev.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<mqueue.h>
#include<unistd.h>

#define DEV_ADDR (0x39)
#define CMD_ID_REGISTER (0x8A)
#define CMD_CONTROL_REGISTER (0x80)
#define CONTROL_REGISTER (0x03)
#define CMD_DATA0LOW_REGISTER_16 (0xAC)
#define CMD_DATA0LOW_REGISTER_8 (0x8C)
#define CMD_DATA0HIGH_REGISTER_8 (0x8D)


int init_lightSensor(void);
void startup_test(int fd);
int write_register(int fd, uint8_t register_type);
uint8_t read_register_8(int fd);
uint8_t* read_register_16(int fd);
void power_up(int fd);
int read_visible_light(int fd);
