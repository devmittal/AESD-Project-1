/*****************************************************************************
 * @file  		temperature.c
 * @brief 		Contains function definitions to communicate with
 *			temperature sensor TMP102.
 * @Comm Type 		I2C
 * @Tools_Used 		ARM-LINUX-GCC
 * @Author(s)		Souvik De, Devansh Mittal
 * @Date 		March 16th, 2019
 * @version 		1.0
*****************************************************************************/

#include "../inc/temperature.h"

#define WRITE_POINTER(fd, reg_address) (write_i2c(fd,reg_address))

tempt_t read_temperature(void)
{
	int fd = 0;
	uint8_t data_low, data_high;
	int16_t temperature_hex = 0;
	tempt_t temperature;

	temperature.IsError = 0;

	fd = init_i2c(TMP102_DEV_ID);
	if(fd < 0)
	{
		temperature.IsError = 1;
		return temperature;
	}

	if(WRITE_POINTER(fd, TEMPERATURE_REG) < 0)
	{
		temperature.IsError = 1;
		return temperature;
	}
	
	data_high = read_i2c8(fd);
	if(data_high < 0)
	{
		temperature.IsError = 1;
		return temperature;
	}

	data_low = read_i2c8(fd);
	if(data_low < 0)
	{
		temperature.IsError = 1;
		return temperature;
	}

	if(close_i2c(fd) < 0)
	{
		temperature.IsError = 1;
		return temperature;
	}

	temperature_hex = ((data_high << 8) | (data_low)) >> 4;

	cal_temp(temperature_hex,&temperature.celcius,&temperature.farenheit,&temperature.kelvin);

	return temperature;
}

void cal_temp(int16_t temperature_hex, float *celcius, float *farenheit, float *kelvin)
{
	if(((temperature_hex & (0x0800)) >> 11))
	{
		temperature_hex ^= (0x0FFF);
		temperature_hex += 1;
		temperature_hex = temperature_hex * (-1);
	}

	*celcius = temperature_hex * SCALING_FACTOR;
	*farenheit = (*celcius * 1.8) + 32;
	*kelvin = *celcius + 273.15;
}

float read_Tlow(void)
{
	int fd = 0;
	uint8_t *data = 0;
	uint16_t tlow_hex = 0;
	float tlow_celsius;

	fd = init_i2c(TMP102_DEV_ID);
	WRITE_POINTER(fd, TLOW_REG);
	data = read_i2c16(fd);
	close_i2c(fd);

	tlow_hex = ((*data << 8) | *(data + 1)) >> 4;
	tlow_celsius = tlow_hex * SCALING_FACTOR;

	return tlow_celsius;
}

float read_Thigh(void)
{
	int fd = 0;
	uint8_t *data = 0;
	uint16_t thigh_hex = 0;
	float thigh_celsius;

	fd = init_i2c(TMP102_DEV_ID);
	WRITE_POINTER(fd, THIGH_REG);
	data = read_i2c16(fd);
	close_i2c(fd);

	thigh_hex = ((*data << 8) | *(data + 1)) >> 4;
	thigh_celsius = thigh_hex * SCALING_FACTOR;

	return thigh_celsius;
}

int read_configuration_reg(void)
{
	int fd = 0;
	uint8_t *data = 0;
	uint16_t configuration_reg_data;

	fd = init_i2c(TMP102_DEV_ID);
	if(fd < 0)
	{
		return -1;
	}

	if(WRITE_POINTER(fd, CONFIGURATION_REG) < 0)
	{
		return -1;
	}

	data = read_i2c16(fd);
	if(data == NULL)
	{
		return -1;
	}

	if(close_i2c(fd) < 0)
	{
		return -1;
	}

	configuration_reg_data = ((*data << 8) | *(data + 1));

	return configuration_reg_data;
}

void set_shutdown(void)
{
	int fd = 0;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	configuration_reg_data = (configuration_reg_data | (0x0100));
	printf("\nConfig: %X | %X",((uint8_t)(configuration_reg_data>>8)),((uint8_t)(configuration_reg_data)));

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c16_config(fd,configuration_reg_data);
	close_i2c(fd);
}

void disable_shutdown(void)
{
	int fd = 0;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	configuration_reg_data = (configuration_reg_data & ~(0x0100));
	printf("\nConfig: %X | %X",((uint8_t)(configuration_reg_data>>8)),((uint8_t)(configuration_reg_data)));

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c16_config(fd,configuration_reg_data);
	close_i2c(fd);
}

uint8_t read_fault(void)
{
	uint8_t fault_bits;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	fault_bits = (uint8_t)((configuration_reg_data & (0x1800)) >> 11);

	return fault_bits;
}

void write_fault(uint8_t fault)
{
	int fd = 0;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	configuration_reg_data &= ~(0x1800); 
	configuration_reg_data |= (fault<<11);

	printf("\nConfig: %X | %X",((uint8_t)(configuration_reg_data>>8)),((uint8_t)(configuration_reg_data)));

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c16_config(fd,configuration_reg_data);
	close_i2c(fd);
}

uint8_t read_em(void)
{
	uint8_t em_bit;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	em_bit = (uint8_t)((configuration_reg_data & (0x0010)) >> 4);

	return em_bit;
}

void write_em(uint8_t em)
{
	int fd = 0;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	configuration_reg_data &= ~(0x0010); 
	configuration_reg_data |= (em<<4);

	printf("\nConfig: %X | %X",((uint8_t)(configuration_reg_data>>8)),((uint8_t)(configuration_reg_data)));

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c16_config(fd,configuration_reg_data);
	close_i2c(fd);
}

uint8_t read_conversion_rate(void)
{
	uint8_t cr;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	cr = (uint8_t)((configuration_reg_data & (0x00C0)) >> 6);

	return cr;
}

void set_conversion_rate(uint8_t cr)
{
	int fd = 0;
	uint16_t configuration_reg_data;	

	configuration_reg_data = read_configuration_reg();

	configuration_reg_data &= ~(0x00C0); 
	configuration_reg_data |= (cr<<6);

	printf("\nConfig: %X | %X",((uint8_t)(configuration_reg_data>>8)),((uint8_t)(configuration_reg_data)));

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c16_config(fd,configuration_reg_data);	
	close_i2c(fd);
}