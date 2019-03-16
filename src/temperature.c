#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "..inc/temperature.h"

int temperature(void)
{
	char *FD_I2C = "/dev/i2c-2";
	int FD;
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
