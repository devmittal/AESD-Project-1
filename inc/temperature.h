/*****************************************************************************
 *   @File		temperature.h
 *   @Brief		Contains dependencies pertaining to temperature.c and
 *			the temperature sensor TMP102.
 *   @Comm Type 	I2C
 *   @Tools_Used 	ARM-LINUX-GCC
 *   @Author(s)		Souvik De, Devansh Mittal
 *   @Date		March 16th, 2019
 *   @version		1.0
*****************************************************************************/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "i2c.h"

#define SCALING_FACTOR	(0.0625)

/* Structure containing data relevant to temperature sensor */
typedef struct Temperature
{
	float celcius;
	float farenheit;
	float kelvin;
	int IsError;
}tempt_t;

/* Pointer Addresses */
enum Registor_Address{TEMPERATURE_REG, CONFIGURATION_REG, TLOW_REG, THIGH_REG};

/**
* @Brief  This function reads the temperature register of the temperature sensor
* @Param  void
* @Return the temperature structure object
**/
tempt_t read_temperature(void);

/**
* @Brief  This function calculates actual temperature in celsius, fahrenheit, and kelvin
*		  based on digital temperature values read from the temperature register
* @Param  temperature_hex: digital temperature values read from the temperature register
		  celcius: store calculated temperature in celsius
		  farenheit: store calculated temperature in fahrenheit
		  kelvin: store calculated temperature in kelvin
* @Return void
**/
void cal_temp(int16_t temperature_hex, float *celcius, float *farenheit, float *kelvin);

/**
* @Brief  This function reads the lower temperature limit from the tlow register
* 		  of the temperature sensor
* @Param  void
* @Return lower temperature limit
**/
float read_Tlow(void);

/**
* @Brief  This function reads the higher temperature limit from the thigh register
* 		  of the temperature sensor
* @Param  void
* @Return higher temperature limit
**/
float read_Thigh(void);

/**
* @Brief  This function reads the configuration register of the temperature sensor
* @Param  void
* @Return configuration register contents or error status
**/
int read_configuration_reg(void);

/**
* @Brief  This function sets the shutdown mode in the configuration register of the
* 		  temperature register
* @Param  void
* @Return void
**/
void set_shutdown(void);

/**
* @Brief  This function disables the shutdown mode in the configuration register of the
* 		  temperature register
* @Param  void
* @Return void
**/
void disable_shutdown(void);

/**
* @Brief  This function reads the fault bits from the configuration register of the
* 		  temperature register
* @Param  void
* @Return value of fault bits
**/
uint8_t read_fault(void);

/**
* @Brief  This function reads the extended mode bit in the configuration register of the
* 		  temperature register
* @Param  void
* @Return Extended Mode bit value
**/
uint8_t read_em(void);

/**
* @Brief  This function sets/disables the extended mode in the configuration register of the
* 		  temperature register
* @Param  em: Normal mode operation(0)/ Extended mode operation(1)
* @Return void
**/
void write_em(uint8_t em);

/**
* @Brief  This function reads the conversion rate bits in the configuration register of the
* 		  temperature register
* @Param  void
* @Return value of conversion rate bits
**/
uint8_t read_conversion_rate(void);

/**
* @Brief  This function sets the conversion rate bits in the configuration register of the
* 		  temperature register
* @Param  cr: conversion rates (0-3)
* @Return void
**/
void set_conversion_rate(uint8_t cr);

#endif
