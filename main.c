/*****************************************************************************
​ ​* ​ ​ @file​ ​  		main.c
​ * ​ ​ @brief​ ​ 		Project-1 AESD (More details to be added soon...)
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include <pthread.h>

#include "inc/i2c.h"
#include "inc/logger.h"
#include "inc/message.h"
#include "inc/lightSensor.h"
#include "inc/temperature.h"

#define NUM_THREADS (3)

typedef struct my_thread
{
	int id;
	char* log;
}thread_t;

void temperature(void *tempeature_thread)
{
	mesg_t message;

	while(1)
	{
		usleep(20000);
		message.temperature = read_temperature();
		sprintf(message.str,"Temperature data read by Temperature Thread (Thread ID = %lu)",syscall(__NR_gettid));
		send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_TEMPERATURE, &message);
		printf("\nTemperature = %f\n",read_temperature().celcius);
	}
}

void light(void *light_thread)
{
	mesg_t message;

	power_up();

	while(1)
	{
		usleep(20000);
		sprintf(message.str,"Light data read by Light Thread (Thread ID = %lu)",syscall(__NR_gettid));
		send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_LIGHT , &message);
		printf("\nVisible Light Lux =%d\n", read_visible_light());
	}
}

void logger(void *logger_thread)
{
	thread_t *logger = (thread_t*) logger_thread;
	write_log(0, logger->log);
	while(1)
	{
		write_log(1, logger->log);
	}
}

int main(int argc, char *argv[])
{
	pthread_t thread[NUM_THREADS];

	thread_t *t_parent = NULL;
	thread_t *t_temperature = NULL;
	thread_t *t_light = NULL;
	thread_t *t_logger = NULL;

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

	t_logger = (thread_t*)malloc(sizeof(thread_t));
	if(t_logger == NULL)
	{
		printf("\nMalloc for Light Thread failed. Exiting\n");
		return 0;
	}
	t_logger->id = thread_id_seed++;
	t_logger->log = argv[1];

	if(pthread_mutex_init(&i2c_bus_lock, NULL) != 0)
	{
		perror("\nI2C Bus Mutex initialization failed. Exiting \n");
		exit(-1);
	}

	if(pthread_create(&thread[0], NULL, (void *)temperature, (void *)t_temperature))
	{
		perror("\nError! Could not create temperature sensor thread: ");
		exit(-1);
	}

	if(pthread_create(&thread[1], NULL, (void *)light, (void *)t_light))
	{
		perror("\nError! Could not create light sensor thread: ");
		exit(-1);
	}	

	if(pthread_create(&thread[2], NULL, (void *)logger, (void *)t_logger))
	{
		perror("\nError! Could not create logger thread: ");
		exit(-1);
	}	

	for(parser = 0 ; parser < NUM_THREADS ; parser++)
	{
		pthread_join(thread[parser],NULL);
	}

	pthread_mutex_destroy(&i2c_bus_lock);

	return 0;
}
