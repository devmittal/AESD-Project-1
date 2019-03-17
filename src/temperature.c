/*****************************************************************************
​ ​* ​ ​ @file​ ​  		temperature.c
​ * ​ ​ @brief​ ​ 		Contains function definations to communicate with
 *					temperature sensor TMP102.
 *	 @Comm Type 	I2C
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/temperature.h"

int init_temperature(void)
{
	char *FD_I2C = "/dev/i2c-2";
	int FD = 0;

	FD = open(FD_I2C, O_RDWR);
	if(FD < 0) 
	{
		perror("Error encountered while openning I2C File Descriptor");
		exit(-1);
	}

	if(ioctl(FD, I2C_SLAVE, TMP102_DEV_ID) < 0) 
	{
		perror("Error encountered while initializing temperature sensor");
		exit(-1);
	}

	return FD;
}

uint8_t write_PointerRegister(int fd, uint8_t register_type)
{
	int value = 0;

	value = write(fd, &register_type, sizeof(register_type));
	if (value < 0)
	{
		perror("Error encountered while writing to pointer register");
	}

	return value;
}

uint8_t* read_PointedRegister(int fd)
{
	int value = 0;
	static uint8_t data[2] = {0};

	value = read(fd, data, sizeof(data));
	if (value != 2)
	{
		perror("Error encountered while reading from pointed register");
		exit(-1);
	}

	return data;
}

float read_temperature(int fd)
{
	uint8_t *data = 0;
	uint16_t temperature_hex = 0;
	float temperature_celcius = 0;

	write_PointerRegister(fd,TEMPERATURE_REG);
	data = read_PointedRegister(fd);

	temperature_hex = ((*data << 8) | *(data + 1)) >> 4;
	temperature_celcius = temperature_hex * SCALING_FACTOR;

	return temperature_celcius;
}

int main()
{
	int fd = init_temperature();
	printf("\n\nTemp = %f\n\n",read_temperature(fd));
	return 0;
}