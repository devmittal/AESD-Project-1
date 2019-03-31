/*****************************************************************************
​ ​* ​ ​ @file​ ​  		main.c
​ * ​ ​ @brief​ ​ 		Project-1 AESD (More details to be added soon...)
 *   @Tools_Used 	ARM-LINUX-GCC
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 16th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "inc/led.h"
#include "inc/i2c.h"
#include "inc/logger.h"
#include "inc/message.h"
#include "inc/temperature.h"

#define NUM_THREADS (3)
#define RECOVERY_DEADLINE (5)

uint8_t isKill = 0;

pthread_t thread[NUM_THREADS];

typedef struct my_thread
{
	int id;
	char* log;
}thread_t;

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
		if(message.light.IsError)
		{
			send_Message(MAIN_QNAME, PRIO_LIGHT , &message);
		}
		else
		{
			if(message.light.isChange)
			{
				sprintf(message.str,"Change in light state read by Light Thread (Thread ID = %lu)",syscall(__NR_gettid));
				send_Message(LOGGR_QNAME, PRIO_LIGHT , &message);	
				send_Message(MAIN_QNAME, PRIO_LIGHT , &message);	
			}
		}
	}
	else if(source == (int *)PRIO_TEMPERATURE)
	{
		message.temperature = read_temperature();
		if(message.temperature.IsError)
		{
			send_Message(MAIN_QNAME, PRIO_TEMPERATURE, &message);
		}
		else
		{
			sprintf(message.str,"Temperature data read by Temperature Thread (Thread ID = %lu)",syscall(__NR_gettid));
			send_Message(LOGGR_QNAME, PRIO_TEMPERATURE, &message);
			send_Message(MAIN_QNAME, PRIO_TEMPERATURE, &message);
			printf("\nTemperature = %f\n",read_temperature().celcius);
		}
	}
	else
	{
		/* For Remote Task */
	}
}


timer_t timer_init(int source, uint8_t interval, void *callback)
{
	struct sigevent sev;
	struct itimerspec trigger;
	static timer_t timerid;
	
	memset(&sev,0,sizeof(struct sigevent));
	memset(&trigger,0,sizeof(struct itimerspec));
	
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = callback;

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
	trigger.it_interval.tv_sec = interval;
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
	//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);
	}
	else
	{
		/* Handle error condition */
	}

	timerid_tmp = timer_init(PRIO_TEMPERATURE, 1, &getSensorData);

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
	//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);		
	}
	else
	{
		/* Handle error */
	}

	power_up();

	timerid_light = timer_init(PRIO_LIGHT, 1, &getSensorData);

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
	mesg_t message;

	message.IsLoggerError = write_log(0, logger->log);
	printf("\nLogger Error = %d and PRIO_LOG = %d\n",message.IsLoggerError, PRIO_LOG );
	send_Message(MAIN_QNAME, PRIO_LOG, &message);

	while(1)
	{
		message.IsLoggerError = write_log(1, logger->log);
		printf("\nLogger Error = %d and PRIO_LOG = %d\n",message.IsLoggerError, PRIO_LOG );
		send_Message(MAIN_QNAME, PRIO_LOG, &message);
	}

}

void check_heartbeat(void)
{
	uint8_t queue_priority;
	uint32_t timetracker = 0;
	uint32_t light_recoverytimeout = 0;
	uint32_t tempt_recoverytimeout = 0;
	mesg_t heartbeatmessage;
	mesg_t message;

	while(1)
	{
		recv_Message(MAIN_QNAME, &queue_priority, &heartbeatmessage);
		printf("\nHello from check_heartbeat : %d\n",queue_priority);

		if(queue_priority == PRIO_LIGHT)
		{
			if(heartbeatmessage.light.IsError)
			{
				led(ON);
				sprintf(message.str,"Heart Beat Alert : Error encountered in Light Thread");
			//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);

				light_recoverytimeout++;
				if(light_recoverytimeout >= RECOVERY_DEADLINE)
				{
					pthread_cancel(thread[1]);
				}
			}
			else
			{
				if((timetracker % 10) == 0)
				{
					led(OFF);
					sprintf(message.str,"Heart Beat Alert : Light Thread working fine");
				//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);
				}
			}
		}
		else if(queue_priority == PRIO_TEMPERATURE)
		{
			if(heartbeatmessage.temperature.IsError)
			{
				led(ON);
				sprintf(message.str,"Heart Beat Alert : Error encountered in Temperature Thread");
			//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);

				tempt_recoverytimeout++;
				if(tempt_recoverytimeout >= RECOVERY_DEADLINE)
				{
					pthread_cancel(thread[0]);
				}
			}
			else
			{
				if((timetracker % 10) == 0)
				{
					led(OFF);
					sprintf(message.str,"Heart Beat Alert : Temperature Thread working fine");
				//	send_Message(LOGGR_QNAME, PRIO_NODATA, &message);
				}
			}
		}
		else if(queue_priority == PRIO_LOG)
		{
			printf("\nHeart Beat Alert : Error Encountered in Logger Thread\n");
			if(heartbeatmessage.IsLoggerError < 0)
			{
				led(ON);
				printf("\nHeart Beat Alert : Error Encountered in Logger Thread\n");
				pthread_cancel(thread[2]);
				isKill = 1;
			}
		}
		else
		{
			/* Check for Remote Task */
		}

		timetracker++;
	}
}

void setup_signalhandler(void)
{
	struct sigaction act;

	memset(&act,0,sizeof(struct sigaction));

	act.sa_handler = &kill_signal_handler;
	if(sigaction(SIGINT,&act,NULL) == -1)
	{
		perror("sigaction: ");
		exit(-1);
	}
}

int main(int argc, char *argv[])
{
	led(OFF);

	thread_t *t_parent = NULL;
	thread_t *t_temperature = NULL;
	thread_t *t_light = NULL;
	thread_t *t_logger = NULL;

	int thread_id_seed = 0;
	int thread_status = 0;
	int parser = 0;

	printf("\nTEMP : %d\nLIGHT: %d\nLOG : %d\nNO : %d\n",PRIO_TEMPERATURE, PRIO_LIGHT, PRIO_LOG, PRIO_NODATA );

	setup_signalhandler();

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

	if(sem_init(&i2c_bus_lock, 0, 1) != 0)
	{
		perror("\nI2C Bus Semaphore initialization failed. Exiting \n");
		exit(-1);	
	}

	init_MessageQueues();

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

	check_heartbeat();	

	for(parser = 0 ; parser < NUM_THREADS ; parser++)
	{
		pthread_join(thread[parser],NULL);
	}

	dest_MessageQueues();

	sem_destroy(&i2c_bus_lock);

	led(OFF);

	return 0;
}
