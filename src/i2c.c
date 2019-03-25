/*****************************************************************************
 * @File  		i2c.c
 * @Brief		Contains function definitions imperative to shared i2c
 *			access by the temperature and light sensor.
 * @COMM		I2C
 * @Tools_Used 		ARM-LINUX-GCC
 * @Author(s)		Souvik De, Devansh Mittal
 * @Date		March 24th, 2019
 * @version 		1.0
*****************************************************************************/

#include "../inc/temperature.h"

int init_i2c(uint8_t DeviceID)
{
	char *FD_I2C = "/dev/i2c-2";
	I2C_FileDescriptor = 0;

	pthread_mutex_lock(&i2c_bus_lock);

	I2C_FileDescriptor = open(FD_I2C, O_RDWR);
	if(I2C_FileDescriptor < 0) 
	{
		perror("Error encountered while opening I2C File Descriptor");
		exit(-1);
	}

	if(ioctl(I2C_FileDescriptor, I2C_SLAVE, DeviceID) < 0) 
	{
		if(DeviceID == TMP102_DEV_ID)
			perror("Error encountered while initializing temperature sensor");
		else
			perror("Error encountered while initializing light sensor");
		exit(-1);
	}

	pthread_mutex_unlock(&i2c_bus_lock);

	return I2C_FileDescriptor;
}

uint8_t write_i2c(int fd, uint8_t register_type)
{
	int value = 0;

	pthread_mutex_lock(&i2c_bus_lock);
	value = write(fd, &register_type, sizeof(register_type));
	pthread_mutex_unlock(&i2c_bus_lock);

	if (value < 0)
	{
		perror("Error encountered in I2C write");
	}

	return value;
}

uint8_t* read_i2c16(int fd)
{
	int value = 0;
	static uint8_t data[2] = {0};

	pthread_mutex_lock(&i2c_bus_lock);
	value = read(fd, data, sizeof(data));
	pthread_mutex_unlock(&i2c_bus_lock);

	if (value != 2)
	{
		perror("Error encountered while reading I2C - 16");
		exit(-1);
	}

	return data;
}

uint8_t read_i2c8(int fd)
{
	uint8_t data = 0;
	int value = 0;

	pthread_mutex_lock(&i2c_bus_lock);
	value = read(fd, data, sizeof(data));
	pthread_mutex_unlock(&i2c_bus_lock);

	if (value != 1)
	{
		perror("Error encountered while reading I2C - 8");
		exit(-1);
	}

	return data;
}

void close_i2c(int fd)
{
	int value = 0;

	pthread_mutex_lock(&i2c_bus_lock);
	value = close(fd);
	pthread_mutex_unlock(&i2c_bus_lock);

	if(value < 0)
	{
		perror("Error encountered while closing I2C - File Descriptor");
		exit(-1);
	}
}
