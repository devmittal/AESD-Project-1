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

#include "../inc/i2c.h"

int init_i2c(uint8_t DeviceID)
{
	char *FD_I2C = "/dev/i2c-2";
	I2C_FileDescriptor = 0;

	sem_wait(&i2c_bus_lock);
	
	I2C_FileDescriptor = open(FD_I2C, O_RDWR);
	if(I2C_FileDescriptor < 0) 
	{
		perror("Error encountered while opening I2C File Descriptor");
		sem_post(&i2c_bus_lock);
		return I2C_FileDescriptor;
	}

	if(ioctl(I2C_FileDescriptor, I2C_SLAVE, DeviceID) < 0) 
	{
		if(DeviceID == TMP102_DEV_ID)
		{
			perror("Error encountered while initializing temperature sensor");
		}
		else
		{
			perror("Error encountered while initializing light sensor");
		}
		sem_post(&i2c_bus_lock);
	}

	return I2C_FileDescriptor;
}

int write_i2c(int fd, uint8_t register_type)
{
	int value = 0;

	value = write(fd, &register_type, sizeof(register_type));
	if (value < 0)
	{
		perror("Error encountered in I2C write");
		sem_post(&i2c_bus_lock);
	}

	return value;
}

int write_i2c16(int fd, uint16_t register_type)
{
	int value = 0;

	value = write(fd, &register_type, sizeof(register_type));
	if (value < 0)
	{
		perror("Error encountered in I2C write - 16");
		sem_post(&i2c_bus_lock);
	}

	return value;
}

uint8_t* read_i2c16(int fd)
{
	int value = 0;
	static uint8_t data[2] = {0};

	value = read(fd, data, sizeof(data));
	if (value != 2)
	{
		perror("Error encountered while reading I2C - 16(2)");
		sem_post(&i2c_bus_lock);
		return NULL;
	}

	return data;
}

int read_i2c8(int fd)
{
	static uint8_t data[1] = {0};
	int value = 0;

	value = read(fd, data, sizeof(data));
	if (value != 1)
	{
		perror("Error encountered while reading I2C - 8");
		sem_post(&i2c_bus_lock);
		return -1;
	}

	return data[0];
}

int close_i2c(int fd)
{
	int value = 0;

	value = close(fd);
	if(value < 0)
	{
		perror("Error encountered while closing I2C - File Descriptor");
	}

	sem_post(&i2c_bus_lock);

	return value;
}
