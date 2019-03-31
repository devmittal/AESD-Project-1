/*****************************************************************************
​ ​* ​ ​ @file​ ​  		remoteTask.c
​ * ​ ​ @brief​ ​ 		Contains function definitions for the remote task.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/remoteTask.h"

int init_socket(void)
{
	int server_FD;
	struct sockaddr_in server_address;
	int socket_attach_status;
	int opt = 1;
	int server_address_length = sizeof(server_address);

	server_FD = socket(AF_INET, SOCK_STREAM, 0);
	if(server_FD == 0)
	{
		perror("Error while creating socket ");
		return -1;
    }
	
	socket_attach_status = setsockopt(server_FD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if(socket_attach_status)
	{
		perror("Error in attaching socket to port");
		return -1;
    }

	server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
	
    if (bind(server_FD, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
    	perror("Error while binding socket");
    	return -1;
    }

   	if (listen(server_FD, 3) < 0)
   	{
   		perror("Error while listening");
   		return -1;
   	}

   	new_socket_FD = accept(server_FD, (struct sockaddr *)&server_address, (socklen_t*)&server_address_length);
    if (new_socket_FD < 0)
    {
    	perror("Error in accept");
    	return -1;
    }

    return 0;	
}

int send_data(mesg_t *message)
{
	if(send(new_socket_FD , (void *)message, sizeof(mesg_t), 0) < 0)
	{
		perror("Error in send ");
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