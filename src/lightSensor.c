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

int startup_test(void)
{
	int id;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	if(fd < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_ID_REGISTER) < 0)
	{
		return -1;
	}

	id = read_i2c8(fd);
	if(id < 0)
	{
		return -1;
	}

	if(close_i2c(fd) < 0)
	{
		return -1;
	}

	return id;
}

int power_up(void)
{
	int control_register_data;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	if(fd < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_CONTROL_REGISTER) < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CONTROL_REGISTER) < 0)
	{
		return -1;
	}

	control_register_data = read_i2c8(fd);
	if(control_register_data < 0)
	{
		return -1;
	}

	if(close_i2c(fd) < 0)
	{
		return -1;
	}

	if(control_register_data == 0x33)
	{
		printf("\nLight Sensor powered up");
		/* print to log file */
	}
	else
	{
		printf("\nLight sensor not powered up! x_x");
		/* print to log file. Handle case??? */
	}

	return control_register_data;
}

uint8_t read_control_register(void)
{
	int fd;
	uint8_t control_register_data;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_CONTROL_REGISTER);		
	control_register_data = read_i2c8(fd);

	close_i2c(fd);

	return control_register_data;
}

uint8_t set_timing_register(uint8_t integ, uint8_t gain)
{
	int fd;
	uint8_t timing_register_data;
	uint8_t new_timing_register_data;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_TIMING_REGISTER);		
	timing_register_data = read_i2c8(fd);	
	
	new_timing_register_data = (timing_register_data & ~(0x13)) | integ | (gain << 4);
	
	write_i2c(fd, new_timing_register_data);

	close_i2c(fd);

	return new_timing_register_data;
}

uint8_t read_timing_register(void)
{
	int fd;
	uint8_t timing_register_data;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_TIMING_REGISTER);		
	timing_register_data = read_i2c8(fd);

	close_i2c(fd);

	return timing_register_data;
}

void enable_interrupt(uint8_t interrupt_persist)
{
	int fd;
	uint8_t interrupt_control_register_data;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_INTERRUPT_CONTROL_REGISTER);

	interrupt_control_register_data = (INTERRUPT_ENABLE) | interrupt_persist;
	write_i2c(fd, interrupt_control_register_data);

	close_i2c(fd);
}

void disable_interrupt(void)
{
	int fd;

	fd = init_i2c(APDS_9301_DEV_ID);
	write_i2c(fd, CMD_INTERRUPT_CONTROL_REGISTER);
	write_i2c(fd, INTERRUPT_DISABLE);

	close_i2c(fd);
}

void set_interrupt_threshold(uint16_t thresh_low, uint16_t thresh_high)
{
	int fd;

	fd = init_i2c(APDS_9301_DEV_ID);
	
	write_i2c(fd, CMD_INTERRUPT_THRESHOLD_REGISTER_LOW);
	write_i2c16(fd, thresh_low);

	write_i2c(fd, CMD_INTERRUPT_THRESHOLD_REGISTER_HIGH);
	write_i2c16(fd, thresh_high);

	close_i2c(fd);
}

uint16_t* read_interrupt_threshold(void)
{
	int fd;
	uint8_t* interrupt_threshold_data;
	static uint16_t final_threshold[2] = {0};

	fd = init_i2c(APDS_9301_DEV_ID);
	
	write_i2c(fd, CMD_INTERRUPT_THRESHOLD_REGISTER_LOW);
	interrupt_threshold_data = read_i2c16(fd);
	final_threshold[0] = (*(interrupt_threshold_data + 1) << 8) | *interrupt_threshold_data;

	write_i2c(fd, CMD_INTERRUPT_THRESHOLD_REGISTER_HIGH);
	interrupt_threshold_data = read_i2c16(fd);	
	close_i2c(fd);

	final_threshold[1] = (*(interrupt_threshold_data + 1) << 8) | *interrupt_threshold_data;

	return final_threshold;
}

int read_visible_light(void)
{
	uint8_t lux_low, lux_high;
	int final_lux;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	if(fd < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_DATA0LOW_REGISTER_8) < 0)
	{
		return -1;
	}

	lux_low = read_i2c8(fd);
	if(lux_low < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_DATA0HIGH_REGISTER_8) < 0)
	{
		return -1;
	}

	lux_high = read_i2c8(fd);
	if(lux_high < 0)
	{
		return -1;
	}

	if(close_i2c(fd) < 0)
	{
		return -1;
	}

	final_lux = (lux_high << 8) | lux_low;

	return final_lux;
}

int read_IR_light(void)
{
	uint8_t lux_low, lux_high;
	int final_lux;
	int fd = 0;

	fd = init_i2c(APDS_9301_DEV_ID);
	if(fd < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_DATA1LOW_REGISTER_8) < 0)
	{
		return -1;
	}

	lux_low = read_i2c8(fd);
	if(lux_low < 0)
	{
		return -1;
	}

	if(write_i2c(fd, CMD_DATA1HIGH_REGISTER_8) < 0)
	{
		return -1;
	}

	lux_high = read_i2c8(fd);
	if(lux_high < 0)
	{
		return -1;
	}

	if(close_i2c(fd) < 0)
	{
		return -1;
	}

	final_lux = (lux_high << 8) | lux_low;

	return final_lux;
}

double cal_lumen(int ch0, int ch1)
{
	double div_result, lux;

	printf("\nch0: %d | ch1: %d",ch0,ch1);
	div_result = (double)ch1/(double)ch0;

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

	return lux;
}

uint8_t state(double lumen)
{
	return (lumen>THRESHOLD) ? 1 : 0;
}

uint8_t change(uint8_t isLight)
{
	static uint8_t previous_state = 0;
	static int isFirstIteration = 1;

	printf("\nLight State: %d",isLight);
	printf("\nPrevious State: %d\n\n",previous_state);

	if(isFirstIteration)
	{
		previous_state = isLight;
		isFirstIteration = 0;
		return 1;
	}
	else
	{
		if(previous_state != isLight)
		{
			previous_state = isLight;
			return 1;
		}
		else
		{
			previous_state = isLight;
			return 0;
		}
	}
}

light_t read_LightSensor(void)
{
	light_t light;

	light.IsError = 0;

	light.lux_visiblelight = read_visible_light();
	if(light.lux_visiblelight < 0)
	{
		light.IsError = 1;
		return light;
	}

	light.lux_irlight = read_IR_light();
	if(light.lux_irlight < 0)
	{
		light.IsError = 1;
		return light;	
	}

	light.lumen = cal_lumen(light.lux_visiblelight, light.lux_irlight);
	light.isLight = state(light.lumen);
	light.isChange = change(light.isLight);
	return light;
}