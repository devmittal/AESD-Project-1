/*****************************************************************************
​ ​* ​ ​ @file​ ​  		unit_test.c
​ * ​ ​ @brief​ ​ 		This file consists of the unit test functionality with
 *					different test cases.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/lightSensor.h"

sem_t i2c_bus_lock;

int test_startup_light(void)
{
	if(startup_test() == 0x50)
		return 0;
	else
		return -1;	
}

int test_powerup_light(void)
{
	if(power_up() == 0x33)
		return 0;
	else
		return -1;
}

int test_timing_register_light(void)
{
	if(set_timing_register(1,1) != read_timing_register())
		return -1;
	if(set_timing_register(2,0) != read_timing_register())	
		return -1;

	return 0;
}

int test_lumen_calculation(void)
{
	if((int)(cal_lumen(5,2)*100000) != (int)(0.066050*100000))
		return -1;
	if((int)(cal_lumen(35,20)*100000) != (int)(0.164000*100000))
		return -1;
	if((int)(cal_lumen(45,30)*100000) != (int)(0.117000*100000))
		return -1;
	if((int)(cal_lumen(100,100)*100000) != (int)(0.034000*100000))
		return -1;
	if((int)(cal_lumen(40,80)*100000) != (int)(0*100000))
		return -1;

	return 0;
}

void register_light_test_suite(int *cnt_success_light, int *cnt_fail_light)
{
	*cnt_success_light = 0;
	*cnt_fail_light = 0;
	printf("\nLight Sensor task Unit Testing\n");

	/* Add tests to the suite */
	if(test_startup_light() == 0)
	{
		printf("\nLight Sensor startup test successful\n");
		(*cnt_success_light)++;
	}
	else
	{
		printf("\nLight Sensor startup test unsuccessful\n");
		(*cnt_fail_light)++;
	}
	if(test_powerup_light() == 0)
	{
		printf("\nLight Sensor power up test successful\n");
		(*cnt_success_light)++;
	}
	else
	{
		printf("\nLight Sensor power up test unsuccessful\n");
		(*cnt_fail_light)++;
	}
	if(test_timing_register_light() == 0)
	{
		printf("\nLight Sensor timing register test successful\n");
		(*cnt_success_light)++;
	}	
	else
	{
		printf("\nLight Sensor timing register test unsuccessful\n");
		(*cnt_fail_light)++;
	}
	if(test_lumen_calculation() == 0)
	{
		printf("\nLumen calculation test successful\n");
		(*cnt_success_light)++;
	}
	else
	{
		printf("\nLumen calculation test unsuccessful\n");
		(*cnt_fail_light)++;
	}
}

int main()
{
	int cnt_success_light, cnt_fail_light;
	printf("\nUnit Testing!\n");
	if(sem_init(&i2c_bus_lock, 0, 1) != 0)
	{
		perror("\nI2C Bus Semaphore initialization failed. Exiting \n");
		exit(-1);	
	}

	register_light_test_suite(&cnt_success_light,&cnt_fail_light);

	printf("\n\nSummary of Unit Testing:\n\n");
	printf("Light Sensor Task Unit Testing Summary:\n");
	printf("Successful cases: %d",cnt_success_light);
	printf("\nUnsuccessful cases: %d\n",cnt_fail_light);
	
	sem_destroy(&i2c_bus_lock);

	return 0;
}