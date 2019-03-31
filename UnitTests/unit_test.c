/*****************************************************************************
​ ​* ​ ​ @file​ ​  		unit_test.c
​ * ​ ​ @brief​ ​ 		This file consists of the unit test functionality with
 *					different test cases.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../inc/lightSensor.h"

sem_t i2c_bus_lock;

int test_startup_light(void)
{
	CU_ASSERT_EQUAL(startup_test(),0x50);
	return 0;
}

int test_powerup_light(void)
{
	CU_ASSERT_EQUAL(power_up(),0x33);	
	return 0;
}

int test_timing_register_light(void)
{
	CU_ASSERT_EQUAL(set_timing_register(1,1),read_timing_register());
	CU_ASSERT_EQUAL(set_timing_register(2,0),read_timing_register());	
	return 0;
}

int test_interrupt_threshold_light(void)
{
	uint16_t* threshold_data;
	
	set_interrupt_threshold(20,2000);
	threshold_data = read_interrupt_threshold();
	CU_ASSERT_EQUAL(*threshold_data,20);
	CU_ASSERT_EQUAL(*(threshold_data+1),2000);

	set_interrupt_threshold(0,0);	
	threshold_data = read_interrupt_threshold();
	CU_ASSERT_EQUAL(*threshold_data,0);
	CU_ASSERT_EQUAL(*(threshold_data+1),0);
	return 0;
}

int test_lumen_calculation(void)
{
	CU_ASSERT_EQUAL(cal_lumen(5,2),0.066050);
	CU_ASSERT_EQUAL(cal_lumen(35,20),0.164000);
	CU_ASSERT_EQUAL(cal_lumen(45,30),0.117000);
	CU_ASSERT_EQUAL(cal_lumen(100,100),0.034000);
	CU_ASSERT_EQUAL(cal_lumen(80,40),0.000000);
	return 0;
}

int register_light_test_suite(void)
{
	printf("\nLight Sensor task Unit Testing\n");

	/* Add tests to the suite */
	if(test_startup_light() == 0)
		printf("Light Sensor startup test successful");
	if(test_powerup_light() == 0)
		printf("Light Sensor power up test successful");
	if()
	   -1 == CU_add_test(light_test_suite, "Light Sensor power up Test", test_powerup_light) ||
	   -1 == CU_add_test(light_test_suite, "Light Sensor Timing register Test", test_timing_register_light) ||
	   -1 == CU_add_test(light_test_suite, "Light Sensor Interrupt Threshold Register Test", test_interrupt_threshold_light) ||
	   -1 == CU_add_test(light_test_suite, "Light Sensor Lumen Calculation", test_lumen_calculation))
		return -1;

	return 0;
}

int main()
{
	printf("\nUnit Testing!\n");
	if(sem_init(&i2c_bus_lock, 0, 1) != 0)
	{
		perror("\nI2C Bus Semaphore initialization failed. Exiting \n");
		exit(-1);	
	}

	if(register_light_test_suite() == -1)
	{
		CU_cleanup_registry();
		CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	if(CUE_SUCCESS != CU_basic_run_tests())
		return CU_get_error();

	CU_set_output_filename("LightSensor");
	CU_automated_run_tests();

	sem_destroy(&i2c_bus_lock);

	/* Clean the registry */
	CU_cleanup_registry();
}