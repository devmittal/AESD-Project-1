/*****************************************************************************
​ ​* ​ ​ @file​ ​  		main.c
​ * ​ ​ @brief​ ​ 		Project-1 AESD (More details to be added soon...)
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <pthread.h>

#include "i2c.h"
#include "inc/logger.h"
#include "inc/message.h"
#include "inc/lightsensor.h"
#include "inc/temperature.h"

#define NUM_THREADS (2)

typedef struct my_thread
{
	int id;
	char* log;
}thread_t;

void temperature(void *tempeature_thread)
{
	while(1)
	{
		printf("\nTemperature = %f\n",read_temperature().celcius);
	}
}

void light(void *temperature_light)
{
	power_up();
	while(1)
	{
		printf("\nVisible Light Lux =%d\n", read_visible_light());
	}
}

int main(int argc, char *argv[])
{
	pthread_t thread[NUM_THREADS];

	thread_t *t_parent = NULL;
	thread_t *t_temperature = NULL;
	thread_t *t_light = NULL;

	int thread_id_seed = 0;
	int thread_status = 0;
	int parser = 0;

	t_parent = (thread_t*)malloc(sizeof(thread_t));
	if(t_parent == NULL)
	{
		printf("\nMalloc for Parent Thread failed. Exiting\n");
		return 0;
	}
	t_parent->id = thread_id_seed++;
	t_parent->log = argv[1];

	t_temperature = (thread_t*)malloc(sizeof(thread_t));
	if(t_temperature == NULL)
	{
		printf("\nMalloc for Temperature Thread failed. Exiting\n");
		return 0;
	}
	t_temperature->id = thread_id_seed++;
	t_temperature->log = argv[1];

	t_light = (thread_t*)malloc(sizeof(thread_t));
	if(t_light == NULL)
	{
		printf("\nMalloc for Light Thread failed. Exiting\n");
		return 0;
	}
	t_light->id = thread_id_seed++;
	t_light->log = argv[1];

	if(pthread_mutex_init(&i2c_bus_lock, NULL) != 0)
	{
		perror("\nI2C Bus Mutex initialization failed. Exiting \n");
		exit(-1);
	}

	if(pthread_create(&threads[0],  (void *)0, temperature, (void *) t_temperature))
	{
		perror("\nError! Could not create temperature thread: ");
		exit(-1);
	}

	if(pthread_create(&threads[1],  (void *)0, light, (void *) t_light))
	{
		perror("\nError! Could not create light sensor thread: ");
		exit(-1);
	}	

	for(parser = 0 ; parser < 4 ; parser++)
	{
		pthread_join(threads[i],NULL);
	}

	pthread_mutex_destroy(&i2c_bus_lock);

	return 0;
}
