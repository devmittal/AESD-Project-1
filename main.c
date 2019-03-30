/*****************************************************************************
​ ​* ​ ​ @file​ ​  		main.c
​ * ​ ​ @brief​ ​ 		Project-1 AESD (More details to be added soon...)
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "inc/i2c.h"
#include "inc/logger.h"
#include "inc/message.h"
#include "inc/temperature.h"

#define NUM_THREADS (3)
uint8_t isKill = 0;

enum LED_STATE{OFF, ON};
pthread_t thread[NUM_THREADS];

typedef struct my_thread
{
	int id;
	char* log;
}thread_t;

/*void led(uint8_t status)
{
	FILE *FD_LED;

	FD_LED = fopen("/sys/devices/platform/leds/leds/beaglebone:green:usr1/brightness", "w");

	if(FD_LED == NULL)
		perror("LED file");
	fprintf(FD_LED, "%d", status);
	fclose(FD_LED);
}*/

void kill_signal_handler(int signum)
{
	int i;
	if(signum == SIGINT)
	{
		isKill = 1;
		pthread_cancel(thread[2]);
	}
}

void getSensorData(union sigval sv)
{
	mesg_t message;
	int *source = sv.sival_ptr;

	if(source == (int *)PRIO_LIGHT)
	{
		message.light = read_LightSensor();
		if(message.light.isChange)
		{
			sprintf(message.str,"Change in light state read by Light Thread (Thread ID = %lu)",syscall(__NR_gettid));
			send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_LIGHT , &message);		
		}
	}
	else
	{
		message.temperature = read_temperature();
		sprintf(message.str,"Temperature data read by Temperature Thread (Thread ID = %lu)",syscall(__NR_gettid));
		send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_TEMPERATURE, &message);
		printf("\nTemperature = %f\n",read_temperature().celcius);
	}
}

timer_t timer_init(int source)
{
	struct sigevent sev;
	struct itimerspec trigger;
	static timer_t timerid;
	
	memset(&sev,0,sizeof(struct sigevent));
	memset(&trigger,0,sizeof(struct itimerspec));
	
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = &getSensorData;

	if(source == PRIO_TEMPERATURE)
		sev.sigev_value.sival_ptr = (void *)PRIO_TEMPERATURE;	
	else
		sev.sigev_value.sival_ptr = (void *)PRIO_LIGHT;

	if(timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
	{
		perror("Timer not created");
		exit(-1);	
	}
	
	trigger.it_value.tv_sec = 1;
	trigger.it_interval.tv_sec = 1;
	if(timer_settime(timerid,0,&trigger,NULL) == -1)
	{
		perror("Timer could not be set");
		exit(-1);
	}

	return timerid;
}

void temperature(void *tempeature_thread)
{
	mesg_t message;
	timer_t timerid_tmp;
	/*printf("\nTLOW: %f",read_Tlow());
	printf("\nTHIGH: %f",read_Thigh());
	//set_shutdown();
	//printf("\nConfig Reg: %X",read_configuration_reg());
	//disable_shutdown();
	//printf("\nConfig Reg: %X",read_configuration_reg());
	write_em(1);
	printf("\nEM Bits: %X",read_em());
	printf("\nConfig Reg: %X",read_configuration_reg());
	set_conversion_rate(3);
	printf("\nCR Bits: %X",read_conversion_rate());
	printf("\nConfig Reg: %X",read_configuration_reg());
	printf("\n");*/

	/* startup test */
	if(read_configuration_reg() == 0x60A0)
	{
		sprintf(message.str,"Temperature sensor I2C working. Startup test successful. (Thread ID = %lu)",syscall(__NR_gettid));
		send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_NODATA, &message);
	}
	else
	{
		/* Handle error condition */
	}

	timerid_tmp = timer_init(PRIO_TEMPERATURE);

	while(1)
	{
		if(isKill == 1)
			break;
	}

	if(timer_delete(timerid_tmp) == -1)
	{
		perror("Timer not deleted");
		exit(-1);
	}
}

void light(void *light_thread)
{
	mesg_t message;
	timer_t timerid_light;

	if(startup_test() == 0x50)
	{
		sprintf(message.str,"Light sensor I2C working. Startup test successful. (Thread ID = %lu)",syscall(__NR_gettid));
		send_Message(LOGGR_QNAME, LOGGR_QSIZE, PRIO_NODATA, &message);		
	}
	else
	{
		/* Handle error */
	}

	power_up();

	timerid_light = timer_init(PRIO_LIGHT);

	while(1)
	{
		if(isKill == 1)
			break;
	}

	if(timer_delete(timerid_light) == -1)
	{
		perror("Timer not deleted");
		exit(-1);
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
	struct sigaction act;

	memset(&act,0,sizeof(struct sigaction));

	act.sa_handler = &kill_signal_handler;
	if(sigaction(SIGINT,&act,NULL) == -1)
	{
		perror("sigaction: ");
		exit(-1);
	}

	//led(0);
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

	/*if(pthread_mutex_init(&i2c_bus_lock, NULL) != 0)
	{
		perror("\nI2C Bus Mutex initialization failed. Exiting \n");
		exit(-1);
	}*/

	if(sem_init(&i2c_bus_lock, 0, 1) != 0)
	{
		perror("\nI2C Bus Semaphore initialization failed. Exiting \n");
		exit(-1);	
	}

	queue_fd = open_MessageQueue(LOGGR_QNAME, LOGGR_QSIZE);

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

	CloseUnlinkQueue(queue_fd, LOGGR_QNAME);

	//pthread_mutex_destroy(&i2c_bus_lock);
	sem_destroy(&i2c_bus_lock);

	return 0;
}
