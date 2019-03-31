/*****************************************************************************
​ ​* ​ ​ @file​ ​  		clientprocess.c
​ * ​ ​ @brief​ ​ 		This process requests information from the main
 *					through a socket.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Souvik De, Devansh Mittal
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/clientprocess.h"

int init_socket(void)
{
	int socket_FD = 0;
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
	if(read( new_socket_FD, message, sizeof(mesg_t)) < 0)
	{
		perror("Error in read data ");
		return -1;
	}

	return 0;
}

int main(void)
{
	
	return 0;
}

