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
#include "../inc/temperature.h"
#include "../inc/logger.h"

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

int test_cal_temperature(void)
{
	float celsius, farenheit, kelvin;

	cal_temp(0x7FF,&celsius,&farenheit,&kelvin);
	if(celsius != 127.937500f || farenheit != 262.287506f || kelvin != 401.087494f)
		return -1;

	cal_temp(0x0,&celsius,&farenheit,&kelvin);	
	if(celsius != 0.0f || farenheit != 32.0f || kelvin != 273.15f)
		return -1;

	cal_temp(0xC90,&celsius,&farenheit,&kelvin);	
	if(celsius != -55.0f || farenheit != -67.0f || kelvin != 218.15f)
		return -1;

	return 0;
}

int test_tlow_register(void)
{
	if(read_Tlow() != 75.0f)
		return -1;

	return 0;
}

int test_thigh_register(void)
{
	if(read_Thigh() != 80.0f)
		return -1;

	return 0;
}

int test_configuration_register(void)
{
	if(read_configuration_reg() != 0x60A0)
		return -1;

	return 0;
}

int test_logfile_creation(void)
{
	if(write_log(0,"test_logfile.txt") != 0)
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

void register_temp_test_suite(int *cnt_success_temp, int *cnt_fail_temp)
{
	*cnt_success_temp = 0;
	*cnt_fail_temp = 0;
	printf("\nTemperature Sensor task Unit Testing\n");

	if(test_cal_temperature() == 0)
	{
		printf("\nActual temperature calculation test successful\n");
		(*cnt_success_temp)++;
	}
	else
	{
		printf("\nActual temperature calculation test unsuccessful\n");
		(*cnt_fail_temp)++;	
	}

	if(test_tlow_register() == 0)
	{
		printf("\nTLow register read test successful\n");
		(*cnt_success_temp)++;	
	}
	else
	{
		printf("\nTlow register read test unsuccessful\n");
		(*cnt_fail_temp)++;	
	}

	if(test_thigh_register() == 0)
	{
		printf("\nThigh register read test successful\n");
		(*cnt_success_temp)++;	
	}
	else
	{
		printf("\nThigh register read test unsuccessful\n");
		(*cnt_fail_temp)++;	
	}

	if(test_configuration_register() == 0)
	{
		printf("\nConfiguration register read test successful\n");
		(*cnt_success_temp)++;	
	}
	else
	{
		printf("\nConfiguration register read test unsuccessful\n");
		(*cnt_fail_temp)++;	
	}
}

void register_logger_test_suite(int *cnt_success_logger, int *cnt_fail_logger)
{
	*cnt_success_logger = 0;
	*cnt_fail_logger = 0;
	printf("\nLogger task Unit Testing\n");

	if(test_logfile_creation() == 0)
	{
		printf("\nLogfile creation test successful\n");
		(*cnt_success_logger)++;		
	}	
	else
	{
		printf("\nLogfile creation test unsuccessful\n");
		(*cnt_fail_logger)++;
	}
}


int main()
{
	int cnt_success_light, cnt_fail_light, cnt_fail_temp, cnt_success_temp, cnt_success_logger, cnt_fail_logger;
	printf("\nUnit Testing!\n");
	if(sem_init(&i2c_bus_lock, 0, 1) != 0)
	{
		perror("\nI2C Bus Semaphore initialization failed. Exiting \n");
		exit(-1);	
	}

	register_light_test_suite(&cnt_success_light,&cnt_fail_light);
	register_temp_test_suite(&cnt_success_temp, &cnt_fail_temp);
	register_logger_test_suite(&cnt_success_logger, &cnt_fail_logger);

	printf("\n\nEnd of Unit Testing");

	printf("\n\nSummary of Unit Testing:\n\n");
	
	printf("Light Sensor Task Unit Testing Summary:\n");
	printf("Total Test Cases: %d\n", (cnt_success_light + cnt_fail_light));
	printf("Successful cases: %d",cnt_success_light);
	printf("\nUnsuccessful cases: %d\n",cnt_fail_light);
	
	printf("\n\nTemperature Sensor Task Unit Testing Summary:\n");
	printf("Total Test Cases: %d\n", (cnt_success_temp + cnt_fail_temp));
	printf("Successful cases: %d",cnt_success_temp);
	printf("\nUnsuccessful cases: %d\n",cnt_fail_temp);
	sem_destroy(&i2c_bus_lock);

	printf("\n\nLogger Task Unit Testing Summary:\n");
	printf("Total Test Cases: %d\n", (cnt_success_logger + cnt_fail_logger));
	printf("Successful cases: %d",cnt_success_logger);
	printf("\nUnsuccessful cases: %d\n",cnt_fail_logger);
	sem_destroy(&i2c_bus_lock);

	printf("\n\nFinal Summary:\n");
	printf("Total Test Cases: %d\n", (cnt_success_logger + cnt_fail_logger + cnt_success_temp + cnt_fail_temp + 
			cnt_success_light + cnt_fail_light));
	printf("Successful cases: %d", (cnt_success_logger + cnt_success_light + cnt_success_temp));
	printf("\nUnsuccessful cases: %d\n",(cnt_fail_logger + cnt_fail_light + cnt_fail_temp));

	return 0;
}