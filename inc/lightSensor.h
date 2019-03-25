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

#include <math.h>
#include "i2c.h"

#define CMD_ID_REGISTER (0x8A)
#define CMD_CONTROL_REGISTER (0x80)
#define CONTROL_REGISTER (0x03)
#define CMD_DATA0LOW_REGISTER_16 (0xAC)
#define CMD_DATA0LOW_REGISTER_8 (0x8C)
#define CMD_DATA0HIGH_REGISTER_8 (0x8D)
#define CMD_DATA1LOW_REGISTER_16 (0xAE)

void startup_test(int fd);
void power_up(int fd);
int read_visible_light(int fd);
void cal_lumen(int ch0, int ch1);
int read_IR_light(int fd);