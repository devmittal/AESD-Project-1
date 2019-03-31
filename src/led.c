/*****************************************************************************
 * @file  		led.c
 * @brief 		Contains function definitions to communicate with BBG LED's.
 * @Tools_Used 		ARM-LINUX-GCC
 * @Author(s)		Souvik De, Devansh Mittal
 * @Date 		March 16th, 2019
 * @version 		1.0
*****************************************************************************/

#include "../inc/led.h"

void led(uint8_t status)
{
	FILE *FD_LED;
	FILE *FD_Directory;
	FILE *FD_Value;

    FD_LED = fopen("/sys/class/gpio/export", "w");
    fseek(FD_LED,0,SEEK_SET);
    fprintf(FD_LED,"%d",53);
    fflush(FD_LED);

    FD_Directory = fopen("/sys/class/gpio/gpio53/direction", "w");
    fseek(FD_Directory,0,SEEK_SET);
    fprintf(FD_Directory,"out");
    fflush(FD_Directory);

    FD_Value = fopen("/sys/class/gpio/gpio53/value", "w");
    fseek(FD_Value,0,SEEK_SET);

    fprintf(FD_Value,"%d",status);
    fflush(FD_Value);

	fclose(FD_Value);
	fclose(FD_Directory);
	fclose(FD_LED);
}