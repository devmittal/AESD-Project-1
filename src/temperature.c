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

tempt_t read_temperature(void)
{
	int fd = 0;
	uint8_t *data = 0;
	uint16_t temperature_hex = 0;
	tempt_t temperature;

	fd = init_i2c(TMP102_DEV_ID);
	write_i2c(fd, TEMPERATURE_REG);
	data = read_i2c16(fd);
	close_i2c(fd);

	temperature_hex = ((*data << 8) | *(data + 1)) >> 4;
	temperature.celcius = temperature_hex * SCALING_FACTOR;
	temperature.farenheit = (temperature.celcius * 1.8) + 32;
	temperature.kelvin = temperature.celcius + 273.15;

	return temperature;
}
