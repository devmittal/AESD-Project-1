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
#define CMD_TIMING_REGISTER (0x81)
#define CMD_INTERRUPT_CONTROL_REGISTER (0x86)
#define CMD_INTERRUPT_THRESHOLD_REGISTER_LOW (0xA2)
#define CMD_INTERRUPT_THRESHOLD_REGISTER_HIGH (0xA4)
#define CMD_DATA0LOW_REGISTER_16 (0xAC)
#define CMD_DATA0LOW_REGISTER_8 (0x8C)
#define CMD_DATA0HIGH_REGISTER_8 (0x8D)
#define CMD_DATA1LOW_REGISTER_16 (0xAE)
#define CMD_DATA1LOW_REGISTER_8 (0x8E)
#define CMD_DATA1HIGH_REGISTER_8 (0x8F)
#define INTERRUPT_ENABLE (0x10)
#define INTERRUPT_DISABLE (0x00)
#define THRESHOLD (1.0)

/* Structure containing data relevant to light sensor */
typedef struct Light
{
	int lux_visiblelight;
	int lux_irlight;
	double lumen;
	uint8_t isLight;
	uint8_t isChange;
	int IsError;
}light_t;

/**
* @Brief  This function checks the identification register as part of a startup test and confirms
* 		  if the light sensor is working properly.
* @Param  void
* @Return ID register contents/Error
**/
int startup_test(void);

/**
* @Brief  This function powers up the light sensor.
* @Param  void
* @Return Control register contents/Error
**/
int power_up(void);

/**
* @Brief  This function reads control register of the light sensor.
* @Param  void
* @Return Control register contents
**/
uint8_t read_control_register(void);

/**
* @Brief  This function is used to set the intergation time and gain of light sensor.
* @Param  integ: Integration time(0-3); gain: gain(0-1)
* @Return New integration time and gain written into timing control register of light sensor
**/
uint8_t set_timing_register(uint8_t integ, uint8_t gain);

/**
* @Brief  This function reads timing register of the light sensor.
* @Param  void
* @Return timing register contents
**/
uint8_t read_timing_register(void);

/**
* @Brief  This function enables interrupts in the interrupt control register of the light sensor.
* @Param  interrupt_persist: value to control when interrupts occur (0-15)
* @Return void
**/
void enable_interrupt(uint8_t interrupt_persist);

/**
* @Brief  This function disable interrupts of the light sensor.
* @Param  void
* @Return void
**/
void disable_interrupt(void);

/**
* @Brief  This function sets the high and low interrupt threshold of the light sensor.
* @Param  thres_low: 16 bit low threshold value; thres_high: 16 bit high threshold value
* @Return void
**/
void set_interrupt_threshold(uint16_t thresh_low, uint16_t thresh_high);

/**
* @Brief  This function reads the high and low interrupt threshold of the light sensor.
* @Param  void
* @Return address of array. 1st element consists of low threshold value. 2nd element of array
* consists of high threshold value
**/
uint16_t* read_interrupt_threshold(void);

/**
* @Brief  This function reads channel 0 of light sensor.
* @Param  void
* @Return 16 bit channel 0 lux value or error status
**/
int read_visible_light(void);

/**
* @Brief  This function reads channel 1 of light sensor.
* @Param  void
* @Return 16 bit channel 1 lux value or error status
**/
int read_IR_light(void);

/**
* @Brief  This function calculates actual luminosity based on channel 0 and channel 1 lux values
*		  of light sensor.
* @Param  ch0: channel 0 lux value; ch1: channel 1 lux value
* @Return actual luminosity
**/
double cal_lumen(int ch0, int ch1);

/**
* @Brief  This function checks if there is any change in light state from the previous state.
* @Param  isLight: Current state (light or dark)
* @Return 1 - change from previous state; 0 - no change from previous state
**/
uint8_t change(uint8_t isLight);

/**
* @Brief  This function determines current light state based on threshold value.
* @Param  lumen: Calculated actual luminosity
* @Return 1 - light; 0 - dark
**/
uint8_t state(double lumen);

/**
* @Brief  This function calls functions to read channel 0 and 1, calculate lumen, determine
*		  current light state, and if there was a change from previous state and stores it
*		  in the allocated structure.
* @Param  void
* @Return the structure object
**/
light_t read_LightSensor(void);