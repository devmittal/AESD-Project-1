/*****************************************************************************
​ ​* ​ ​ @file​ ​  		lightSensor.c
​ * ​ ​ @brief​ ​ 		Contains function definitions to communicate with
 *			light sensor APDS-9301.
 *   @Comm Type 	I2C
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 24th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/lightSensor.h"

void startup_test(int fd)
{
	uint8_t id;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_ID_REGISTER);

	id = read_i2c8(fd);
	if(id == 0x50)
	{
		printf("\nLight sensor Functional");
		/* print to log file */
	}
	else
	{
		printf("\nLight sensor not functional");
		/* print to log file */
	}

	close_i2c(fd);
}

void power_up(int fd)
{
	uint8_t control_register_data;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_CONTROL_REGISTER);
	write_i2c(fd, CONTROL_REGISTER);

	control_register_data = read_i2c8(fd);
	if(control_register_data == 0x33)
	{
		printf("\nLight Sensor powered up");
		/* print to log file */
	}
	else
	{
		printf("\nLight sensor not powered up! x_x");
		/* print to log file */
	}

	close_i2c(fd);
}

int read_visible_light(int fd)
{
	uint8_t* lux;
	//uint8_t lux1, lux2;
	int final_lux;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	
	/*write_register(fd, CMD_DATA0LOW_REGISTER_8);
	lux1 = read_register_8(fd); 
	write_register(fd, CMD_DATA0HIGH_REGISTER_8);
	lux2 = read_register_8(fd);

	printf("\n%X | %X",lux1,lux2);

	final_lux1 = (lux2 << 8) | lux1;

	printf("\nChannel 0 Lux - %d | %X",final_lux1,final_lux1);*/

	write_i2c(fd, CMD_DATA0LOW_REGISTER_16);
	lux = read_i2c16(fd);

	close_i2c(fd);

	final_lux = (*(lux + 1) << 8) | *lux;

	printf("\nChannel 0 Lux - %d | %X", final_lux, final_lux);

	return final_lux;
}

int read_IR_light(int fd)
{
	uint8_t* lux;
	int final_lux;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_DATA1LOW_REGISTER_16);
	lux = read_i2c16(fd);

	close_i2c(fd);

	final_lux = (*(lux + 1) << 8) | *lux;

	printf("\nChannel 1 Lux - %d | %X", final_lux, final_lux);

	return final_lux;
}

void cal_lumen(int ch0, int ch1)
{
	double div_result, lux;

	printf("\nch0: %d | ch1: %d",ch0,ch1);
	div_result = (double)ch1/(double)ch0;
	printf("\ndiv_result: %f",div_result);

	if(div_result > 0 && div_result <= 0.50)
		lux = (.0304 * ch0) - (.062 * ch0 * pow(div_result,1.4));
	else if(div_result > 0.5 && div_result <= 0.61)
		lux = (0.0224 * ch0) - (.031 * ch1);
	else if(div_result > 0.61 && div_result <= 0.80)
		lux = (0.0128 * ch0) - (.0153 * ch1);
	else if(div_result > 0.80 && div_result <= 1.30)
		lux = (0.00146 * ch0) - (.00112 * ch1);
	else
		lux = 0;

	printf("\nLumen: %f",lux);
}

int main(void)
{
	int ch0, ch1;
	startup_test(fd);
	power_up(fd);
	ch0 = read_visible_light(fd);
	ch1 = read_IR_light(fd);
	cal_lumen(ch0,ch1);
	return 0;
}
