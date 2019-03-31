/*****************************************************************************
​ ​* ​ ​ @file​ ​  		remoteTask.c
​ * ​ ​ @brief​ ​ 		Contains function definitions for the remote task.
 *   @Tools_Used 	Compiler: ARM-LINUX-GCC; Board: BeagleBone Green
​ * ​ ​ @Author(s)​  	​​Devansh Mittal, Souvik De
​ * ​ ​ @Date​ ​​ 		March 30th, 2019
​ * ​ ​ @version​ ​ 		1.0
*****************************************************************************/

#include "../inc/remoteTask.h"

void init_socket(void)
{
	int socketfd, client_len, client_socketfd;;
	struct sockaddr_in socket_str, client_addr;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1)
	{
		perror("Socket Creation failed: ");
		exit(-1);
	}

	bzero((char *) &socket_str, sizeof(socket_str));
	
	socket_str.sin_family = AF_INET;
	socket_str.sin_addr.s_addr = INADDR_ANY;
	socket_str.sin_port = htons(PORTNO);

	if(bind(socketfd, (struct sockaddr *) &socket_str, sizeof(socket_str)) == -1)
	{
		perror("Binding error: ");
		exit(-1);
	}

	if(listen(socketfd,5) == -1)
	{
		perror("Listening error: ");
		exit(-1);
	}

	client_len = sizeof(client_addr);
	client_socketfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_len);
	if(client_socketfd == -1)
	{
		perror("Client socket not received: ");
		exit(-1);
	}
}