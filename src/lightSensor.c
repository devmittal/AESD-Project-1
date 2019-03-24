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

int init_lightSensor(void)
{
	char *FD_I2C = "/dev/i2c-2";
	int FD = 0;

	FD = open(FD_I2C, O_RDWR);
	if(FD < 0) 
	{
		perror("Error encountered while opening I2C File Descriptor");
		exit(-1);
	}

	if(ioctl(FD, I2C_SLAVE, DEV_ADDR) < 0) 
	{
		perror("Error encountered while initializing light sensor");
		exit(-1);
	}

	return FD;
}

int write_register(int fd, uint8_t register_type)
{
	int status = 0;

	status = write(fd, &register_type, sizeof(register_type));
	if (status < 0)
	{
		perror("Error encountered while writing to register");
	}

	return status;
}

uint8_t read_register_8(int fd)
{
	static uint8_t data[1] = {0};
	int status;

	status = read(fd, data, sizeof(data));
	if (status != 1)
	{
		perror("Error encountered while reading from register");
		exit(-1);
	}

	return data[0];
}

uint8_t* read_register_16(int fd)
{
	static uint8_t data[2] = {0};
	int status;

	status = read(fd, data, sizeof(data));
	if (status != 2)
	{
		perror("Error encountered while reading from register");
		exit(-1);
	}

	return data;
}

void startup_test(int fd)
{
	uint8_t id;

	write_register(fd, CMD_ID_REGISTER);
	id = read_register_8(fd);

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
}

void power_up(int fd)
{
	uint8_t control_register_data;

	write_register(fd, CMD_CONTROL_REGISTER);
	write_register(fd, CONTROL_REGISTER);
	control_register_data = read_register_8(fd);

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
}

int read_visible_light(int fd)
{
	uint8_t* lux;
	//uint8_t lux1, lux2;
	int final_lux;
	
	/*write_register(fd, CMD_DATA0LOW_REGISTER_8);
	lux1 = read_register_8(fd); 
	write_register(fd, CMD_DATA0HIGH_REGISTER_8);
	lux2 = read_register_8(fd);

	printf("\n%X | %X",lux1,lux2);

	final_lux1 = (lux2 << 8) | lux1;

	printf("\nChannel 0 Lux - %d | %X",final_lux1,final_lux1);*/

	write_register(fd, CMD_DATA0LOW_REGISTER_16);
	lux = read_register_16(fd);

	final_lux = (*(lux + 1) << 8) | *lux;

	printf("\nChannel 0 Lux - %d | %X", final_lux, final_lux);

	return final_lux;
}

int read_IR_light(int fd)
{
	uint8_t* lux;
	int final_lux;

	write_register(fd, CMD_DATA1LOW_REGISTER_16);
	lux = read_register_16(fd);

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
	int fd;
	int ch0, ch1;
	fd = init_lightSensor();
	startup_test(fd);
	power_up(fd);
	ch0 = read_visible_light(fd);
	ch1 = read_IR_light(fd);
	cal_lumen(ch0,ch1);
	return 0;
}
