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

		if(strcmp(tolower(request), "temperature") == 0)
		{
			message.str
		
			if(send_data)

			printf("Choose from the following temperature options :\n");
			printf("1) Celsius\n");
			printf("2) Farenheit\n");
			printf("3) Kelvin\n\n");

			printf("Option : ");
			scanf("%d", item);	
		}
		else if(strcmp(tolower(request), "temperature") == 0)
		{
			printf("Choose from the following light options :\n");
			printf("1) Lux Visible\n");
			printf("2) Lux IR\n");
			printf("3) Luminosity\n");
			printf("4) Light State\n");
		}
	}

	return 0;
}

