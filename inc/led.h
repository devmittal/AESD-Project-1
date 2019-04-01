/*****************************************************************************
​ ​* ​ ​ @file​ ​  		led.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to BBG User LED's.
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 29th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#ifndef LED_H
#define LED_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Denotes if the LED is ON or OFF */
enum LED_STATE{OFF, ON};

/**
* @Brief  This function switches the LED on and off
* @Param  ON-1 or OFF-0
* @Return void
**/
void led(uint8_t status);

#endif