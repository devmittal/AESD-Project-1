#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "inc/temperature.h"
#include "inc/logger.h"

#define NUM_THREADS (4)

pthread_t threads[NUM_THREADS]; //Thread descriptors

int main(int argc, char *argv[])
{
	char *filename;
	int i;

	filename = argv[1];

	if(pthread_create(&threads[0],  (void *)0, temperature, (void *)0)) // parameters to pass in
	{
		perror("\nError! Could not create temperature thread: ");
		exit(1);
	}

	if(pthread_create(&threads[1],  (void *)0, light_sensor, (void *)0)) // parameters to pass in
	{
		perror("\nError! Could not create light sensor thread: ");
		exit(1);
	}	

	if(pthread_create(&threads[2],  (void *)0, remote_request_socket, (void *)0)) // parameters to pass in
	{
		perror("\nError! Could not create remote request socket thread: ");
		exit(1);
	}

	if(pthread_create(&threads[i],  (void *)0, logger, (void *)filename)) // parameters to pass in
	{
		perror("\nError! Could not create logger thread: ");
		exit(1);
	}

	for(i=0;i<3;i++)
		pthread_join(threads[i],NULL);

	return 0;
}