#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bind_socket.h"

void bind_socket(int current_socket, struct addrinfo *addr_info)
{

	if ( bind(current_socket, addr_info->ai_addr, addr_info->ai_addrlen))
	{
		perror("Bind to port");
		exit(-1);
	}
}


