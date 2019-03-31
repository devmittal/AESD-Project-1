/*****************************************************************************
​ ​* ​ ​ @file​ ​  		lightSensor.h
​ * ​ ​ @brief​ ​ 		Contains dependencies pertaining to lightSensor.c and
 *					the light sensor APDS-9301.
 *					Note: Addresses prepended with "CMD" indicates a write to
 *					the command register
 *	 @Comm Type 	I2C
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 24th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <math.h>
#include "i2c.h"

#define CMD_ID_REGISTER (0x8A)
#define CMD_CONTROL_REGISTER (0x80)
#define CONTROL_REGISTER (0x03)
#define CMD_TIMING_REGISTER (0x81)
#define CMD_INTERRUPT_CONTROL_REGISTER (0x86)
#define CMD_INTERRUPT_THRESHOLD_REGISTER_LOW (0xA2)
#define CMD_INTERRUPT_THRESHOLD_REGISTER_HIGH (0xA4)
#define CMD_DATA0LOW_REGISTER_16 (0xAC)
#define CMD_DATA0LOW_REGISTER_8 (0x8C)
#define CMD_DATA0HIGH_REGISTER_8 (0x8D)
#define CMD_DATA1LOW_REGISTER_16 (0xAE)
#define CMD_DATA1LOW_REGISTER_8 (0x8E)
#define CMD_DATA1HIGH_REGISTER_8 (0x8F)
#define INTERRUPT_ENABLE (0x10)
#define INTERRUPT_DISABLE (0x00)
#define THRESHOLD (1.0)

typedef struct Light
{
	int lux_visiblelight;
	int lux_irlight;
	double lumen;
	uint8_t isLight;
	uint8_t isChange;
}light_t;

uint8_t startup_test(void);
uint8_t power_up(void);
uint8_t read_control_register(void);
uint8_t set_timing_register(uint8_t integ, uint8_t gain);
uint8_t read_timing_register(void);
void enable_interrupt(uint8_t interrupt_persist);
void disable_interrupt(void);
void set_interrupt_threshold(uint16_t thresh_low, uint16_t thresh_high);
uint16_t* read_interrupt_threshold(void);
int read_visible_light(void);
int read_IR_light(void);
double cal_lumen(int ch0, int ch1);
uint8_t change(uint8_t isLight);
uint8_t state(double lumen);
light_t read_LightSensor(void);