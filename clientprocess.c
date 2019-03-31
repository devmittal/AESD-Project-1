/*****************************************************************************
​ ​* ​ ​ @file​ ​  		clientprocess.c
​ * ​ ​ @brief​ ​ 		This process requests information from the main
 *					through a socket.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "inc/clientprocess.h"

int init_socket(void)
{
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;

	socket_FD = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_FD < 0)
    {
       	perror("Error in creating socket");
       	return -1;
    }
  
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
      
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
	{
        	perror("\nUnsupported Address\n");
        	return -1;
	}

   	if (connect(socket_FD, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
   	{
       	perror("Error in Connection");
       	return -1;
   	}

   	return 0;
}

int send_data(mesg_t *message)
{
	if(send(socket_FD, (void *)message, sizeof(mesg_t), 0) < 0)
	{
		return -1;
	}

	return 0;
}

int read_data(mesg_t *message)
{
	if(read(socket_FD, message, sizeof(mesg_t)) < 0)
	{
		perror("Error in read data ");
		return -1;
	}

	return 0;
}

int main(void)
{
	char request[20] = {0};
	int item = 0;
	mesg_t message;

	if(init_socket() < 0)
	{
		printf("Socket Initialization Failed! Exiting :(\n");
		exit(-1);
	}

	while(1)
	{
		printf("Enter request type : ");
		scanf("%s", request);	

		if(strcmp(request, "temperature") == 0)
		{
			sprintf(message.str,"temperature");
			if(send_data(&message) < 0)
			{
				printf("Failed to send data to server. Exiting :(\n");
				exit(-1);
			}

			if(read_data(&message) < 0)
			{
				printf("Failed to read data from server. Exiting :(\n");
				exit(-1);
			}

			printf("Choose from the following temperature options :\n");
			printf("1) Celsius\n");
			printf("2) Farenheit\n");
			printf("3) Kelvin\n\n");

			printf("Option : ");
			scanf("%d", item);

			switch(item)
			{
				case 1:	printf("Temperature in Celsuis = %f\n", message.temperature.celcius);
						break;

				case 2:	printf("Temperature in Farenheit = %f\n", message.temperature.farenheit);
						break;

				case 3:	printf("Temperature in Kelvin = %f\n", message.temperature.kelvin);
						break;

				default: printf("Invalid Item Number ! Try again\n");
			}	
		}
		else if(strcmp(request, "light") == 0)
		{
			sprintf(message.str,"light");
			if(send_data(&message) < 0)
			{
				printf("Failed to send data to server. Exiting :(\n");
				exit(-1);
			}

			if(read_data(&message) < 0)
			{
				printf("Failed to read data from server. Exiting :(\n");
				exit(-1);
			}

			printf("Choose from the following light options :\n");
			printf("1) Lux Visible\n");
			printf("2) Lux IR\n");
			printf("3) Luminosity\n");
			printf("4) Light State\n\n");

			printf("Option : ");
			scanf("%d", item);

			switch(item)
			{
				case 1:	printf("Lux Visible Light = %d\n", message.light.lux_visiblelight);
						break;

				case 2:	printf("Lux IR Light = %d\n", message.light.lux_irlight);
						break;

				case 3:	printf("Luminosity = %d\n", message.light.lumen);
						break;

				case 4:	if(message.light.isLight)
							printf("Light State = Light\n");
						else
							printf("Light State = Dark\n");
						break;

				default: printf("Invalid Item Number ! Try again\n");
			}	
		}
		else
		{
			printf("Invalid request! Try again.\n");
		}
	}

	return 0;
}

